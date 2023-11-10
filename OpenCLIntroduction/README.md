## Install dependences for Ubintu 20.04.

Check your OS release
```
lsb_release -r
```

```
mkdir tmp && cd tmp
wget https://github.com/intel/intel-graphics-compiler/releases/download/igc-1.0.12149.1/intel-igc-core_1.0.12149.1_amd64.deb
wget https://github.com/intel/intel-graphics-compiler/releases/download/igc-1.0.12149.1/intel-igc-opencl_1.0.12149.1_amd64.deb
wget https://github.com/intel/compute-runtime/releases/download/22.39.24347/intel-level-zero-gpu-dbgsym_1.3.24347_amd64.ddeb
wget https://github.com/intel/compute-runtime/releases/download/22.39.24347/intel-level-zero-gpu_1.3.24347_amd64.deb
wget https://github.com/intel/compute-runtime/releases/download/22.39.24347/intel-opencl-icd-dbgsym_22.39.24347_amd64.ddeb
wget https://github.com/intel/compute-runtime/releases/download/22.39.24347/intel-opencl-icd_22.39.24347_amd64.deb
wget https://github.com/intel/compute-runtime/releases/download/22.39.24347/libigdgmm12_22.2.0_amd64.deb
```

```
sudo apt install ./*.deb
```