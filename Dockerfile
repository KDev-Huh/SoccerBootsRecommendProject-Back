# 1. 빌드 스테이지
FROM ubuntu:22.04 AS builder

# 패키지 설치 시 대화창 뜨는 것 방지
ENV DEBIAN_FRONTEND=noninteractive

# 빌드 의존성 설치
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    ninja-build \
    libasio-dev \
    wget \
    ca-certificates \
    && rm -rf /var/lib/apt/lists/*

# CMake 최신 버전 설치 (3.29 이상 확보)
RUN wget https://github.com/Kitware/CMake/releases/download/v3.29.0/cmake-3.29.0-linux-x86_64.sh && \
    chmod +x cmake-3.29.0-linux-x86_64.sh && \
    ./cmake-3.29.0-linux-x86_64.sh --skip-license --prefix=/usr/local && \
    rm cmake-3.29.0-linux-x86_64.sh

# ... (ONNX 설치 및 나머지 빌드 과정 동일)

# ONNX Runtime 설치
WORKDIR /tmp
RUN wget -q https://github.com/microsoft/onnxruntime/releases/download/v1.17.1/onnxruntime-linux-x64-1.17.1.tgz && \
    tar -xzf onnxruntime-linux-x64-1.17.1.tgz && \
    cp -r onnxruntime-linux-x64-1.17.1/include/* /usr/local/include/ && \
    cp -r onnxruntime-linux-x64-1.17.1/lib/* /usr/local/lib/ && \
    ldconfig

# 소스 복사 및 빌드
WORKDIR /app
COPY . .

# build 폴더를 새로 만들어 Release 모드로 빌드
RUN mkdir -p build && cd build && \
    cmake -G Ninja -DCMAKE_BUILD_TYPE=Release .. && \
    ninja soccer_boots_bayes_project

# 2. 실행 스테이지
FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

# 실행에 필요한 최소 라이브러리 설치
RUN apt-get update && apt-get install -y \
    libasio-dev \
    ca-certificates \
    && rm -rf /var/lib/apt/lists/*

# 빌드 스테이지에서 생성된 ONNX 라이브러리 복사
COPY --from=builder /usr/local/lib/libonnxruntime* /usr/local/lib/
RUN ldconfig

WORKDIR /app

# 실행 파일 및 데이터 파일 복사 (경로 단순화)
COPY --from=builder /app/build/soccer_boots_bayes_project .
COPY --from=builder /app/datasets ./datasets
COPY --from=builder /app/model ./model

# 실행 권한 부여
RUN chmod +x ./soccer_boots_bayes_project

EXPOSE 8080

# 서버 실행
CMD ["./soccer_boots_bayes_project"]