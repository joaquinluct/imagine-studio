# check-encoding.ps1
# Script to verify UTF-8 with BOM encoding and CRLF line endings in text files

param(
    [switch]$Fix = $false
)

$ErrorCount = 0
$FixedCount = 0

Write-Host "========================================" -ForegroundColor Cyan
Write-Host "Imagine Studio - Encoding Checker" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""

# Define text file extensions to check
$extensions = @("*.md", "*.cpp", "*.h", "*.hpp", "*.c", "*.hlsl", "*.cmake", "*.txt", "*.json", "*.yml", "*.yaml", "*.xml", "*.ini", "*.bat", "*.ps1")

# Get all text files in the project (excluding .git, build, x64 directories)
$files = Get-ChildItem -Path . -Include $extensions -Recurse -File | 
    Where-Object { $_.FullName -notmatch '\\\.git\\|\\build\\|\\x64\\|\\Debug\\|\\Release\\' }

Write-Host "Checking $($files.Count) text files..." -ForegroundColor Yellow
Write-Host ""

foreach ($file in $files) {
    $relativePath = $file.FullName.Replace((Get-Location).Path + "\", "")
    
    # Check for UTF-8 with BOM
    $bytes = [System.IO.File]::ReadAllBytes($file.FullName)
    $hasBOM = ($bytes.Length -ge 3 -and $bytes[0] -eq 0xEF -and $bytes[1] -eq 0xBB -and $bytes[2] -eq 0xBF)
    
    # Check for mixed line endings
    $content = [System.IO.File]::ReadAllText($file.FullName)
    $hasCRLF = $content -match "`r`n"
    $hasLFOnly = $content -match "(?<!`r)`n"
    $hasMixedLineEndings = $hasCRLF -and $hasLFOnly
    
    # Check for non-ASCII characters
    $hasNonASCII = $content -match '[^\x00-\x7F]'
    
    $hasIssues = $false
    
    if (-not $hasBOM -and $hasNonASCII) {
        Write-Host "? $relativePath" -ForegroundColor Red
        Write-Host "   Missing UTF-8 BOM (file contains non-ASCII characters)" -ForegroundColor Red
        $hasIssues = $true
        $ErrorCount++
    }
    
    if ($hasMixedLineEndings) {
        if (-not $hasIssues) {
            Write-Host "? $relativePath" -ForegroundColor Red
        }
        Write-Host "   Mixed line endings detected (CRLF + LF)" -ForegroundColor Red
        $hasIssues = $true
        $ErrorCount++
    }
    
    if ($hasIssues) {
        if ($Fix) {
            try {
                # Read content and normalize
                $content = Get-Content $file.FullName -Raw -Encoding UTF8
                
                # Normalize line endings to CRLF (for .sh files use LF)
                if ($file.Extension -eq ".sh") {
                    $content = $content -replace "`r`n", "`n"
                } else {
                    $content = $content -replace "(?<!`r)`n", "`r`n"
                }
                
                # Write with UTF-8 BOM
                $utf8BOM = New-Object System.Text.UTF8Encoding $true
                [System.IO.File]::WriteAllText($file.FullName, $content, $utf8BOM)
                
                Write-Host "   ? FIXED" -ForegroundColor Green
                $FixedCount++
            }
            catch {
                Write-Host "   ??  FAILED TO FIX: $_" -ForegroundColor Yellow
            }
        }
        Write-Host ""
    }
}

Write-Host ""
Write-Host "========================================" -ForegroundColor Cyan
Write-Host "Summary" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan

if ($ErrorCount -eq 0) {
    Write-Host "? All files have correct encoding and line endings!" -ForegroundColor Green
    exit 0
} else {
    Write-Host "? Found $ErrorCount file(s) with encoding issues" -ForegroundColor Red
    
    if ($Fix) {
        Write-Host "? Fixed $FixedCount file(s)" -ForegroundColor Green
        Write-Host ""
        Write-Host "Run 'git add --renormalize .' to update Git index" -ForegroundColor Yellow
    } else {
        Write-Host ""
        Write-Host "Run with -Fix flag to automatically fix issues:" -ForegroundColor Yellow
        Write-Host "  .\scripts\check-encoding.ps1 -Fix" -ForegroundColor Cyan
    }
    
    exit 1
}

