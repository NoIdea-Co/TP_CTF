FROM python:3.10-slim

RUN adduser --disabled-password --gecos "" ctfuser \
    && apt-get update \
    && apt-get install -y --no-install-recommends ca-certificates \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app
COPY requirements.txt .
RUN pip install --no-cache-dir -r requirements.txt
COPY . .
USER ctfuser
ENV PORT=5006
EXPOSE 5006
CMD ["python", "app.py"]
