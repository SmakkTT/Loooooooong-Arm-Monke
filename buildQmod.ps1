# Builds a .zip file for loading with BMBF
$NDKPath = Get-Content $PSScriptRoot/ndkpath.txt

$buildScript = "$NDKPath/build/ndk-build"
if (-not ($PSVersionTable.PSEdition -eq "Core")) {
    $buildScript += ".cmd"
}

& $buildScript NDK_PROJECT_PATH=$PSScriptRoot APP_BUILD_SCRIPT=$PSScriptRoot/Android.mk NDK_APPLICATION_MK=$PSScriptRoot/Application.mk -j 4
Compress-Archive -Path  "./libs/arm64-v8a/liblongarmmonke.so",`
                        "./extern/libbeatsaber-hook_2_2_4.so",`
                        "./extern/libbeatsaber-hook_2_2_5.so",`
                        "./extern/libcustom-types.so",`
                        "./cover.png",`
                        "./mod.json" -DestinationPath "./LongArmMonke.zip" -Update 

& copy-item -Force "./LongArmMonke.zip" "./LongArmMonke-1.0.0.qmod"