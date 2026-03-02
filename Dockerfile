# Multi-stage build for Soccer Boots Recommender API
FROM ubuntu:22.04 AS builder

# Install build dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    ninja-build \
    libasio-dev \
    wget \
    ca-certificates \
    && rm -rf /var/lib/apt/lists/*

# Install ONNX Runtime
WORKDIR /tmp
RUN wget https://github.com/microsoft/onnxruntime/releases/download/v1.17.1/onnxruntime-linux-x64-1.17.1.tgz && \
    tar -xzf onnxruntime-linux-x64-1.17.1.tgz && \
    cp -r onnxruntime-linux-x64-1.17.1/include/* /usr/local/include/ && \
    cp -r onnxruntime-linux-x64-1.17.1/lib/* /usr/local/lib/ && \
    ldconfig && \
    rm -rf /tmp/onnxruntime*

# Set working directory
WORKDIR /app

# Copy source code
COPY . .

# Create build directory and build
WORKDIR /app/cmake-build-debug
RUN cmake -G Ninja .. && \
    cmake --build . --target soccer_boots_bayes_project

# Runtime stage
FROM ubuntu:22.04

# Install runtime dependencies
RUN apt-get update && apt-get install -y \
    libasio-dev \
    ca-certificates \
    && rm -rf /var/lib/apt/lists/*

# Copy ONNX Runtime libraries from builder
COPY --from=builder /usr/local/lib/libonnxruntime* /usr/local/lib/
RUN ldconfig

# Set working directory
WORKDIR /app/cmake-build-debug

# Copy built executable and necessary files
COPY --from=builder /app/cmake-build-debug/soccer_boots_bayes_project .
COPY --from=builder /app/datasets /app/datasets
COPY --from=builder /app/model /app/model

# Expose API port
EXPOSE 8080

# Run the server
CMD ["./soccer_boots_bayes_project"]
