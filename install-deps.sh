#!/usr/bin/env bash
set -e

echo "==> Detecting package manager and installing system dependencies..."
if command -v apt-get &> /dev/null; then
    # Debian / Ubuntu / Pop!_OS
    sudo apt-get update
    sudo apt-get install -y \
        build-essential \
        git \
        libasound2-dev \
        libx11-dev \
        libxrandr-dev \
        libxi-dev \
        libgl1-mesa-dev \
        libglu1-mesa-dev \
        libxcursor-dev \
        libxinerama-dev \
        libwayland-dev \
        libxkbcommon-dev
elif command -v dnf &> /dev/null; then
    # Fedora / RHEL
    sudo dnf install -y \
        gcc \
        make \
        git \
        alsa-lib-devel \
        libX11-devel \
        libXrandr-devel \
        libXi-devel \
        mesa-libGL-devel \
        mesa-libGLU-devel \
        libXcursor-devel \
        libXinerama-devel \
        wayland-devel \
        libxkbcommon-devel
else
    echo "==> Warning: Unsupported package manager. Please install Raylib system dependencies manually."
fi

echo "==> System dependencies installed successfully!"
echo "==> You can now run 'make' to clone Raylib, build it, and compile your project."
