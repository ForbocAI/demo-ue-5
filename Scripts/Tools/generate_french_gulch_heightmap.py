#!/usr/bin/env python3
"""Generate the French Gulch, CA terrain sample used by the runtime demo.

The source is Mapzen/AWS Terrarium elevation tiles. Elevation is decoded in
meters with:

    height = (red * 256 + green + blue / 256) - 32768

The runtime mesh consumes the compact CSV. The PNG files are for inspection and
future Unreal Landscape import work.
"""

from __future__ import annotations

import csv
import json
import math
import os
from pathlib import Path
from typing import Iterable
from urllib.request import urlopen
from urllib.parse import urlencode

from PIL import Image


POST_OFFICE_LAT = 40.701291
POST_OFFICE_LON = -122.638465
ZOOM = 14
TILE_SIZE = 256
EXTENT_METERS = 3200.0
CSV_GRID_SIZE = 65
HEIGHTMAP_SIZE = 513
TILE_URL_TEMPLATE = (
    "https://s3.amazonaws.com/elevation-tiles-prod/terrarium/{z}/{x}/{y}.png"
)

ROOT = Path(__file__).resolve().parents[2]
OUT_DIR = ROOT / "Content" / "Map" / "Heightmaps"
CSV_PATH = OUT_DIR / "french_gulch_ca_terrain_65.csv"
HEIGHTMAP_PATH = OUT_DIR / "french_gulch_ca_heightmap_513.png"
PREVIEW_PATH = OUT_DIR / "french_gulch_ca_terrain_preview.png"
ORTHO_PATH = OUT_DIR / "french_gulch_ca_usgs_ortho_1024.png"
ORTHO_CSV_PATH = OUT_DIR / "french_gulch_ca_ortho_65.csv"
METADATA_PATH = OUT_DIR / "french_gulch_ca_terrain_metadata.json"
ORTHO_EXPORT_URL = (
    "https://basemap.nationalmap.gov/arcgis/rest/services/"
    "USGSImageryOnly/MapServer/export"
)


def latlon_to_pixel(lat: float, lon: float, zoom: int) -> tuple[float, float]:
    sin_lat = math.sin(math.radians(lat))
    scale = TILE_SIZE * (2**zoom)
    x = (lon + 180.0) / 360.0 * scale
    y = (
        0.5
        - math.log((1.0 + sin_lat) / (1.0 - sin_lat)) / (4.0 * math.pi)
    ) * scale
    return x, y


def pixel_to_latlon(x: float, y: float, zoom: int) -> tuple[float, float]:
    scale = TILE_SIZE * (2**zoom)
    lon = x / scale * 360.0 - 180.0
    merc_n = math.pi - 2.0 * math.pi * y / scale
    lat = math.degrees(math.atan(math.sinh(merc_n)))
    return lat, lon


def meters_per_pixel(lat: float, zoom: int) -> float:
    return 156543.03392804097 * math.cos(math.radians(lat)) / (2**zoom)


def decode_terrarium(pixel: tuple[int, int, int] | tuple[int, int, int, int]) -> float:
    red, green, blue = pixel[:3]
    return (red * 256.0 + green + blue / 256.0) - 32768.0


def fetch_tile(z: int, x: int, y: int) -> Image.Image:
    url = TILE_URL_TEMPLATE.format(z=z, x=x, y=y)
    print(f"Downloading {url}")
    with urlopen(url, timeout=30) as response:
        return Image.open(response).convert("RGB").copy()


def fetch_ortho(bounds: dict[str, float]) -> Image.Image:
    query = urlencode(
        {
            "bbox": (
                f"{bounds['west']},{bounds['south']},"
                f"{bounds['east']},{bounds['north']}"
            ),
            "bboxSR": "4326",
            "imageSR": "4326",
            "size": "1024,1024",
            "format": "png32",
            "transparent": "false",
            "f": "image",
        }
    )
    url = f"{ORTHO_EXPORT_URL}?{query}"
    print(f"Downloading {url}")
    with urlopen(url, timeout=60) as response:
        return Image.open(response).convert("RGB").copy()


def tile_range(min_px: float, max_px: float) -> range:
    return range(math.floor(min_px / TILE_SIZE), math.floor(max_px / TILE_SIZE) + 1)


def build_mosaic(
    min_px: float, max_px: float, min_py: float, max_py: float
) -> tuple[Image.Image, int, int, list[str]]:
    tile_xs = list(tile_range(min_px, max_px))
    tile_ys = list(tile_range(min_py, max_py))
    origin_x = tile_xs[0] * TILE_SIZE
    origin_y = tile_ys[0] * TILE_SIZE

    mosaic = Image.new("RGB", (len(tile_xs) * TILE_SIZE, len(tile_ys) * TILE_SIZE))
    urls: list[str] = []

    for tile_y in tile_ys:
        for tile_x in tile_xs:
            tile = fetch_tile(ZOOM, tile_x, tile_y)
            mosaic.paste(
                tile,
                ((tile_x - tile_xs[0]) * TILE_SIZE, (tile_y - tile_ys[0]) * TILE_SIZE),
            )
            urls.append(TILE_URL_TEMPLATE.format(z=ZOOM, x=tile_x, y=tile_y))

    return mosaic, origin_x, origin_y, urls


def sample_elevation(
    image: Image.Image, origin_x: int, origin_y: int, global_x: float, global_y: float
) -> float:
    local_x = max(0.0, min(image.width - 1.0, global_x - origin_x))
    local_y = max(0.0, min(image.height - 1.0, global_y - origin_y))
    x0 = int(math.floor(local_x))
    y0 = int(math.floor(local_y))
    x1 = min(x0 + 1, image.width - 1)
    y1 = min(y0 + 1, image.height - 1)
    tx = local_x - x0
    ty = local_y - y0

    h00 = decode_terrarium(image.getpixel((x0, y0)))
    h10 = decode_terrarium(image.getpixel((x1, y0)))
    h01 = decode_terrarium(image.getpixel((x0, y1)))
    h11 = decode_terrarium(image.getpixel((x1, y1)))

    top = h00 * (1.0 - tx) + h10 * tx
    bottom = h01 * (1.0 - tx) + h11 * tx
    return top * (1.0 - ty) + bottom * ty


def sample_grid(
    image: Image.Image,
    origin_x: int,
    origin_y: int,
    min_px: float,
    max_px: float,
    min_py: float,
    max_py: float,
    size: int,
) -> list[list[float]]:
    rows: list[list[float]] = []
    for row in range(size):
        y = min_py + (max_py - min_py) * row / (size - 1)
        values: list[float] = []
        for col in range(size):
            x = min_px + (max_px - min_px) * col / (size - 1)
            values.append(sample_elevation(image, origin_x, origin_y, x, y))
        rows.append(values)
    return rows


def flatten(rows: Iterable[Iterable[float]]) -> list[float]:
    return [value for row in rows for value in row]


def write_csv(rows: list[list[float]]) -> None:
    with CSV_PATH.open("w", newline="", encoding="utf-8") as handle:
        handle.write("# French Gulch, CA terrain sample\n")
        handle.write("# Source: Mapzen/AWS Terrarium elevation tiles\n")
        handle.write(
            f"# Post office lat/lon: {POST_OFFICE_LAT:.6f}, {POST_OFFICE_LON:.6f}\n"
        )
        handle.write(f"# Extent meters: {EXTENT_METERS:.1f}\n")
        handle.write("# Units: elevation meters\n")
        writer = csv.writer(handle)
        for row in rows:
            writer.writerow([f"{value:.3f}" for value in row])


def write_heightmap(rows: list[list[float]], path: Path) -> None:
    values = flatten(rows)
    min_h = min(values)
    max_h = max(values)
    span = max(max_h - min_h, 0.001)

    image = Image.new("I;16", (len(rows[0]), len(rows)))
    pixels = [round((value - min_h) / span * 65535.0) for value in values]
    image.putdata(pixels)
    image.save(path)


def color_relief(value: float, min_h: float, max_h: float) -> tuple[int, int, int]:
    t = (value - min_h) / max(max_h - min_h, 0.001)
    stops = [
        (0.00, (45, 121, 78)),
        (0.25, (106, 169, 88)),
        (0.45, (202, 190, 101)),
        (0.65, (181, 125, 75)),
        (0.82, (150, 88, 81)),
        (1.00, (224, 222, 210)),
    ]
    for idx in range(1, len(stops)):
        left_t, left_color = stops[idx - 1]
        right_t, right_color = stops[idx]
        if t <= right_t:
            local = (t - left_t) / max(right_t - left_t, 0.001)
            return tuple(
                round(left_color[channel] * (1.0 - local) + right_color[channel] * local)
                for channel in range(3)
            )
    return stops[-1][1]


def write_preview(rows: list[list[float]], path: Path) -> None:
    values = flatten(rows)
    min_h = min(values)
    max_h = max(values)
    image = Image.new("RGB", (len(rows[0]), len(rows)))
    image.putdata([color_relief(value, min_h, max_h) for value in values])
    image = image.resize((1024, 1024), Image.Resampling.BICUBIC)
    image.save(path)


def write_ortho_csv(image: Image.Image) -> None:
    with ORTHO_CSV_PATH.open("w", newline="", encoding="utf-8") as handle:
        handle.write("# French Gulch, CA USGS ortho imagery sample\n")
        handle.write("# Source: USGSImageryOnly MapServer export\n")
        handle.write("# Units: sRGB 0-255\n")
        writer = csv.writer(handle)
        for row in range(CSV_GRID_SIZE):
            y = round((image.height - 1) * row / (CSV_GRID_SIZE - 1))
            cells: list[str] = []
            for col in range(CSV_GRID_SIZE):
                x = round((image.width - 1) * col / (CSV_GRID_SIZE - 1))
                red, green, blue = image.getpixel((x, y))
                cells.append(f"{red}:{green}:{blue}")
            writer.writerow(cells)


def write_metadata(
    csv_rows: list[list[float]],
    heightmap_rows: list[list[float]],
    bounds: dict[str, float],
    tile_urls: list[str],
) -> None:
    values = flatten(csv_rows)
    heightmap_values = flatten(heightmap_rows)
    metadata = {
        "name": "French Gulch, California post-office-centered terrain sample",
        "post_office": {"lat": POST_OFFICE_LAT, "lon": POST_OFFICE_LON},
        "bounds": bounds,
        "extent_meters": EXTENT_METERS,
        "zoom": ZOOM,
        "csv_grid_size": CSV_GRID_SIZE,
        "heightmap_size": HEIGHTMAP_SIZE,
        "elevation_units": "meters",
        "csv_min_elevation_m": round(min(values), 3),
        "csv_max_elevation_m": round(max(values), 3),
        "heightmap_min_elevation_m": round(min(heightmap_values), 3),
        "heightmap_max_elevation_m": round(max(heightmap_values), 3),
        "tile_format": "terrarium",
        "tile_url_template": TILE_URL_TEMPLATE,
        "tile_format_reference": "https://github.com/tilezen/joerd/blob/master/docs/formats.md",
        "ortho_source": "USGSImageryOnly MapServer",
        "ortho_export_url": ORTHO_EXPORT_URL,
        "tile_urls": tile_urls,
        "generated_by": os.path.relpath(__file__, ROOT),
    }
    METADATA_PATH.write_text(json.dumps(metadata, indent=2) + "\n", encoding="utf-8")


def main() -> None:
    OUT_DIR.mkdir(parents=True, exist_ok=True)

    center_px, center_py = latlon_to_pixel(POST_OFFICE_LAT, POST_OFFICE_LON, ZOOM)
    half_extent_px = EXTENT_METERS / meters_per_pixel(POST_OFFICE_LAT, ZOOM) / 2.0
    min_px = center_px - half_extent_px
    max_px = center_px + half_extent_px
    min_py = center_py - half_extent_px
    max_py = center_py + half_extent_px

    mosaic, origin_x, origin_y, tile_urls = build_mosaic(min_px, max_px, min_py, max_py)

    csv_rows = sample_grid(
        mosaic, origin_x, origin_y, min_px, max_px, min_py, max_py, CSV_GRID_SIZE
    )
    heightmap_rows = sample_grid(
        mosaic,
        origin_x,
        origin_y,
        min_px,
        max_px,
        min_py,
        max_py,
        HEIGHTMAP_SIZE,
    )

    north, west = pixel_to_latlon(min_px, min_py, ZOOM)
    south, east = pixel_to_latlon(max_px, max_py, ZOOM)
    bounds = {
        "north": round(north, 7),
        "south": round(south, 7),
        "west": round(west, 7),
        "east": round(east, 7),
    }

    write_csv(csv_rows)
    write_heightmap(heightmap_rows, HEIGHTMAP_PATH)
    write_preview(heightmap_rows, PREVIEW_PATH)
    ortho = fetch_ortho(bounds)
    ortho.save(ORTHO_PATH)
    write_ortho_csv(ortho)
    write_metadata(csv_rows, heightmap_rows, bounds, tile_urls)

    print(f"Wrote {CSV_PATH}")
    print(f"Wrote {HEIGHTMAP_PATH}")
    print(f"Wrote {PREVIEW_PATH}")
    print(f"Wrote {ORTHO_PATH}")
    print(f"Wrote {ORTHO_CSV_PATH}")
    print(f"Wrote {METADATA_PATH}")


if __name__ == "__main__":
    main()
