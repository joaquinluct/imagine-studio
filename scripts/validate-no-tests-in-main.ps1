# Pre-commit validation script
# Ensures tests are NOT included in main project

$vcxproj = "Imagine Studio.vcxproj"

if (-not (Test-Path $vcxproj)) {
    Write-Host "? Error: $vcxproj not found" -ForegroundColor Red
    exit 1
}

$content = Get-Content $vcxproj -Raw

# Check if any test files are in main project
if ($content -match '<ClCompile Include="tests\\[^"]+\.cpp"') {
    Write-Host "? VALIDATION FAILED: Test files found in main project!" -ForegroundColor Red
    Write-Host ""
    Write-Host "Tests MUST be separate executables, not part of main project." -ForegroundColor Yellow
    Write-Host "This causes LNK2005 error (multiple main() functions)." -ForegroundColor Yellow
    Write-Host ""
    Write-Host "Fixing automatically..." -ForegroundColor Cyan
    
    # Auto-fix: Remove test files
    $content = $content -replace '\s*<ClCompile Include="tests\\[^"]+\.cpp" />\s*', ''
    Set-Content $vcxproj $content -NoNewline
    
    Write-Host "? Test files removed from main project" -ForegroundColor Green
    Write-Host ""
    Write-Host "Please verify with:" -ForegroundColor Yellow
    Write-Host "  msbuild 'Imagine Studio.sln' /t:Build /p:Configuration=Debug /p:Platform=x64" -ForegroundColor White
    exit 0
}

Write-Host "? Validation passed: No test files in main project" -ForegroundColor Green
exit 0
