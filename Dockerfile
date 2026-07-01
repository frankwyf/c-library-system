# ---- Build stage ----
FROM gcc:16 AS builder

LABEL org.opencontainers.image.source="https://github.com/frankwyf/c-library-system"
LABEL org.opencontainers.image.description="C Library Management System"
LABEL org.opencontainers.image.licenses="MIT"

RUN apt-get update && apt-get install -y --no-install-recommends \
    cmake make && \
    rm -rf /var/lib/apt/lists/*

WORKDIR /src
COPY . .

RUN cmake -B build -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTS=OFF && \
    cmake --build build --parallel

# ---- Test stage (optional: docker build --target test .) ----
FROM builder AS test

RUN cmake -B build-test -DCMAKE_BUILD_TYPE=Debug -DBUILD_TESTS=ON && \
    cmake --build build-test --parallel && \
    ctest --test-dir build-test --output-on-failure -V

# ---- Runtime stage (minimal image) ----
FROM debian:bookworm-slim AS runtime

LABEL org.opencontainers.image.source="https://github.com/frankwyf/c-library-system"
LABEL org.opencontainers.image.description="C Library Management System — runtime"
LABEL org.opencontainers.image.licenses="MIT"

RUN apt-get update && apt-get install -y --no-install-recommends \
    libc6 && \
    rm -rf /var/lib/apt/lists/*

WORKDIR /app
COPY --from=builder /src/build/library .
COPY --from=builder /src/data ./data

ENTRYPOINT ["./library"]
CMD ["data/books.txt"]
