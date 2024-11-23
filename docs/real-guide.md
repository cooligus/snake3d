# How to build it on Debian based distro

## Install dependencies

```bash
sudo apt install libglew-dev
sudo apt install libglm-dev
sudo apt install libglfw3-dev
sudo apt install cmake
```

## Build it

```bash
git clone https://github.com/cooligus/snake3d
cd snake3d
cmake -S . -B build
cd build
make
```
Now you should see output executable called Snake3D