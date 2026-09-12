# build.ps1 — compila player + testes, roda todos os testes, faz cleanup.
#
# Uso: .\scripts\build.ps1
# Flags opcionais:
#   -SkipTests   só compila, não roda os testes
#   -KeepExe     não apaga player.exe no fim (útil pra debug)

param(
    [switch]$SkipTests,
    [switch]$KeepExe
)

$ErrorActionPreference = "Stop"
$env:Path = [Environment]::GetEnvironmentVariable("Path", "User") + ";" + $env:Path

$Flags = "-std=c11", "-Wall", "-Wextra", "-Wpedantic", "-Werror"
$SrcFiles = "src/main.c", "src/playlist.c", "src/io.c"
$Tests = @(
    @{Name="test_add";       File="tests/test_playlist_add.c"},
    @{Name="test_nav";       File="tests/test_playlist_navigation.c"},
    @{Name="test_display";   File="tests/test_playlist_display.c"},
    @{Name="test_search";    File="tests/test_playlist_search_sort.c"},
    @{Name="test_fronteira"; File="tests/test_fronteira_texto.c"}
)

# Limpa artefatos de execuções anteriores (mantém o repo limpo).
Remove-Item *.exe, *.out, tmp_*.in, tmp_*.in.out -Force -ErrorAction SilentlyContinue

Write-Host "==> Compilando player.exe" -ForegroundColor Cyan
& gcc @Flags $SrcFiles -o player.exe
if ($LASTEXITCODE -ne 0) { throw "compilação do player falhou" }

if ($SkipTests) {
    Write-Host "(testes pulados por -SkipTests)" -ForegroundColor Yellow
    return
}

foreach ($t in $Tests) {
    Write-Host "==> Compilando $($t.Name).exe" -ForegroundColor Cyan
    & gcc @Flags -I src $t.File src/playlist.c -o "$($t.Name).exe"
    if ($LASTEXITCODE -ne 0) { throw "compilação de $($t.Name) falhou" }
}

$failures = @()
foreach ($t in $Tests) {
    Write-Host "==> Rodando $($t.Name)" -ForegroundColor Cyan
    & ".\$($t.Name).exe"
    if ($LASTEXITCODE -ne 0) { $failures += $t.Name }
}

if ($failures.Count -gt 0) {
    Write-Host "`nFALHA: $($failures -join ', ')" -ForegroundColor Red
    exit 1
}

Write-Host "`nTodos os testes passaram." -ForegroundColor Green

# Cleanup automático.
$artifacts = Get-Item *.exe, *.out, tmp_*.in, tmp_*.in.out -ErrorAction SilentlyContinue
if (-not $KeepExe) {
    Remove-Item *.exe, *.out, tmp_*.in, tmp_*.in.out -Force -ErrorAction SilentlyContinue
    Write-Host "Executáveis e artefatos removidos." -ForegroundColor DarkGray
}
