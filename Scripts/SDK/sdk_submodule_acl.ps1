param(
  [Parameter(Mandatory = $true)]
  [ValidateSet("apply", "remove")]
  [string] $Mode,

  [Parameter(Mandatory = $true)]
  [string] $Path
)

$ErrorActionPreference = "Stop"

$GeneratedDirs = @("Binaries", "Intermediate", "Saved")
$Identity = [System.Security.Principal.WindowsIdentity]::GetCurrent().Name
$Root = (Resolve-Path -LiteralPath $Path).Path
$DenyRule = "${Identity}:(WD,AD,DC)"

function Clear-ReadOnlyAttributes {
  $RootItem = Get-Item -LiteralPath $Root -Force
  $RootItem.Attributes = $RootItem.Attributes -band (-bnot [System.IO.FileAttributes]::ReadOnly)

  Get-ChildItem -LiteralPath $Root -Force -Recurse | ForEach-Object {
    $_.Attributes = $_.Attributes -band (-bnot [System.IO.FileAttributes]::ReadOnly)
  }
}

function Ensure-GeneratedDirectories {
  foreach ($Name in $GeneratedDirs) {
    New-Item -ItemType Directory -Force -Path (Join-Path $Root $Name) | Out-Null
  }
}

function Is-GeneratedPath([string] $Candidate) {
  foreach ($Name in $GeneratedDirs) {
    $GeneratedRoot = Join-Path $Root $Name
    if (($Candidate -eq $GeneratedRoot) -or
        $Candidate.StartsWith($GeneratedRoot + [System.IO.Path]::DirectorySeparatorChar)) {
      return $true
    }
  }
  return $false
}

function Read-LockDirectories {
  @($Root) + @(
    Get-ChildItem -LiteralPath $Root -Directory -Recurse -Force |
      Where-Object { -not (Is-GeneratedPath $_.FullName) } |
      ForEach-Object { $_.FullName }
  )
}

function Remove-WriteDeny {
  foreach ($Dir in Read-LockDirectories) {
    & icacls.exe $Dir /remove:d "$Identity" /C | Out-Null
    $Code = $LASTEXITCODE
    if (($Code -ne 0) -and ($Code -ne 2)) {
      exit $Code
    }
  }
}

function Apply-WriteDeny {
  foreach ($Dir in Read-LockDirectories) {
    & icacls.exe $Dir /deny $DenyRule /C | Out-Null
    if ($LASTEXITCODE -ne 0) {
      exit $LASTEXITCODE
    }
  }
}

Clear-ReadOnlyAttributes

if ($Mode -eq "remove") {
  Remove-WriteDeny
  exit 0
}

Ensure-GeneratedDirectories
Remove-WriteDeny
Apply-WriteDeny
