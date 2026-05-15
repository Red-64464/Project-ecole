param(
    [int]$TrainGames = 200,
    [int]$CompareGames = 20,
    [int]$DemoGames = 5,
    [string]$Layout = "smallClassic",
    [int]$Depth = 2,
    [string]$Python = "python",
    [switch]$Train
)

$ErrorActionPreference = "Stop"
Set-StrictMode -Version Latest

$ScriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
Push-Location $ScriptDir

function Invoke-Step {
    param(
        [string]$Title,
        [scriptblock]$Command
    )

    Write-Host "============================================"
    Write-Host $Title
    Write-Host "============================================"
    $global:LASTEXITCODE = $null
    & $Command
    if ($null -ne $LASTEXITCODE -and $LASTEXITCODE -ne 0) {
        throw "Step failed: $Title"
    }
}

try {
    if ($Train) {
        Invoke-Step "Step 1: Train RL weights" {
            & $Python train.py --num-games $TrainGames --depth $Depth --layout $Layout
        }
    }
    else {
        Write-Host "============================================"
        Write-Host "Step 1: Train RL weights skipped"
        Write-Host "============================================"
        Write-Host "Using existing final weights from weights.json."
        Write-Host "Add -Train if you really want to overwrite them."
    }

    Write-Host ""
    Invoke-Step "Step 2: Compare agents" {
        & $Python compare.py --num-games $CompareGames --layout $Layout --depth $Depth
    }

    Write-Host ""
    Invoke-Step "Step 3: Demo RLMinimax" {
        & $Python pacman.py -p RLMinimaxAgent -l $Layout -n $DemoGames -q
    }

    Write-Host ""
    Write-Host "============================================"
    Write-Host "Pipeline completed successfully."
    Write-Host "Weights used from weights.json."
    Write-Host "============================================"
}
finally {
    Pop-Location
}
