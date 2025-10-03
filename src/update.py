import os

import requests

from tqdm import tqdm

REPO_AUTHOR = 'filcherock'
REPO_NAME = 'block.note'
CURRENT_VERSION = 'v0.1.0'

def _checkVersionJsonInfo():
    url = f'https://api.github.com/repos/{REPO_AUTHOR}/{REPO_NAME}/releases/latest'
    response = requests.get(url)
    response.raise_for_status() 
    return response.json()

def checkUpdate():
    latest_release = _checkVersionJsonInfo()
    latest_version = latest_release['tag_name']
    if CURRENT_VERSION != latest_release:
        installUpdate(latest_version)
    else:
        print('Actual')

def installUpdate(lv):
    url = f'https://github.com/{REPO_AUTHOR}/{REPO_NAME}/releases/download/{lv}/BlockNote'
    response = requests.get(url, stream=True)
    total_size = int(response.headers.get('content-length', 0))
    file_path = os.path.join(os.getcwd(), 'bnote')
    with open(file_path, 'wb') as file, tqdm(total=total_size, unit='B', unit_scale=True, desc='Downloading') as bar:
        for data in response.iter_content(chunk_size=1024):
            file.write(data)
            bar.update(len(data))

if __name__ == '__main__':
    checkUpdate()