#!/bin/sh
set -eu
socat TCP-LISTEN:6009,fork,reuseaddr EXEC:"/app/fmt" &
exec python app.py
