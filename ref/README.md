# French Gulch Reference Images

This folder stores visual planning references for the UE runtime. These files are
reference material only; do not treat third-party screenshots as production
texture sources unless their rights and provenance are cleared.

## Current References

- `Untitled - Google Earth - Google Chrome 2026-06-25 (2).png`: French Gulch
  County Park / Clear Creek visual reference for creek banks, riparian
  vegetation, rocks, and hillsides.
- `frontier-of-jefferson-french-gulch.jpg`: Frontier of Jefferson French Gulch
  tone reference for stamp-mill, boardwalk, mud, saloon, boarding-house,
  company-office, and hard-rock mining settlement dressing.
- `frontier-of-jefferson-french-gulch-road.jpg`: Frontier of Jefferson road
  tone reference for the single freight/mail route, ruts, wagon traffic, brush,
  and approach pacing.
- `french-gulch-usps-google-maps-2026-07-07-1.png`: Google Maps reference for
  the U.S. Post Office / 14200 Trinity Mountain Road street-facing context.
- `french-gulch-usps-google-maps-2026-07-07-2.png`: Google Maps reference for
  post-office scale, parking/road edge, and adjacent building placement.
- `french-gulch-usps-google-maps-2026-07-07-3.png`: Google Maps reference for
  frontage orientation and Main Street spacing around the post-office anchor.
- `Buell-RDR2.png`: horse silhouette and tack reference for the town horse
  pass.
- `a-picture-showing-two-horses-from-red-dead-redemption-2.png`: multi-horse
  spacing, posture, and pacing reference for primitive horse routes.

## Implementation Notes

- Use the horse images as visual references for proportions and pacing only.
- Keep runtime prototype horses in project-owned primitive actors until the staged
  source horse assets in `pre-game-assets` are imported and licensed for use.
- Use the Clear Creek image with the public USGS ortho/terrain outputs when
  planning the Water System, Foliage Tool, and PCG natural environment pass.
- Use the Google Maps screenshots as layout/proportion references only; do not
  convert them into runtime textures.
- Use the Frontier of Jefferson images and text as art-direction references for
  selecting and placing licensed/project-owned assets.
- Screenshot verification for issues should be saved under
  `../classified/screenshots`.
