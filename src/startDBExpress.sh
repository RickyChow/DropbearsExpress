#!/bin/bash
echo Starting Dropbears Express FCGI
spawn-fcgi -p 8000 -n build/DBExpress
