source .venv/bin/activate
g++ -o dist/bnote src/app.cpp -lncurses
pyinstaller -F src/update.py 