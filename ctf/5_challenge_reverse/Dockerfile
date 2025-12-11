FROM python:3.10-slim

RUN adduser --disabled-password --gecos "" ctfuser \
    && apt-get update \
    && apt-get install -y --no-install-recommends build-essential ca-certificates \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app
COPY requirements.txt .
RUN pip install --no-cache-dir -r requirements.txt
COPY . .

# Build the binary and keep as downloadable artifact
RUN gcc -O2 -s -o rev_check rev_check.c

USER ctfuser
ENV PORT=5007
EXPOSE 5007
CMD ["python", "app.py"]
