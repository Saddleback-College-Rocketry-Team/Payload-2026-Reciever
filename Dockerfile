# Use Ubuntu as base
FROM ubuntu:22.04

# Set environment variables to avoid interactive prompts
ENV DEBIAN_FRONTEND=noninteractive
ENV LANG=C.UTF-8

# Update packages and install essentials
RUN apt-get update && apt-get install -y \
    git \
    python3 \
    python3-pip \
    curl \
    unzip \
    build-essential \
    && rm -rf /var/lib/apt/lists/*

# Install PlatformIO
RUN pip3 install --no-cache-dir platformio

# Install Teensy platform
RUN pio platform install teensy

# Set working directory inside container
WORKDIR /workspace

# Copy your project files into the container
COPY . /workspace

# Optional: pre-install libraries defined in platformio.ini
RUN pio lib install

# Default command
CMD ["pio", "run"]