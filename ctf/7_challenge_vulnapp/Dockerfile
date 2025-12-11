FROM python:3.10-slim

RUN adduser --disabled-password --gecos "" ctfuser \
    && apt-get update \
    && apt-get install -y --no-install-recommends build-essential socat ca-certificates \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app
COPY requirements.txt .
RUN pip install --no-cache-dir -r requirements.txt
COPY . .
RUN gcc -O2 -s -o fmt fmt.c && chmod +x entrypoint.sh
USER ctfuser
ENV PORT=5009
EXPOSE 5009 6009
CMD ["/bin/sh", "entrypoint.sh"]
