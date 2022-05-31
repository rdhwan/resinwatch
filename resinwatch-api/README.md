# ResinWatch-API
The RESTapi of ResinWatch to provide the needed data.

## Modules used
- FastApi
- Uvicorn/Gunicorn
- Genshin.py (need cookies, read the Cookies section below)

## Cookies
Cookies are needed for genshin.py to get the realtime notes of your account.
To get your cookies, please refer to [how to get hoyolab cookies](https://thesadru.github.io/genshin.py/authentication/). Also make sure your profile/realtime notes is set to public in hoyolab.

## Installation
1. Create `.env` file then fill with these parameters : 
```env
APP_USERNAME="resinwatchv1"
APP_PASSWORD="YOUR_SUPER_DUPER_SECURE_DESIRED_PASSWORD"
UID=GENSHIN_ACCOUNT_UID
LTUID=COOKIES_LTUID
LTOKEN="COOKIES_LTOKEN"
```
To get the LTUID and LTOKEN, please refer to the Cookies section.

2. Install required dependencies
```bash
pip install -r requirements.txt
```
3. Run by executing main.py or direcly run uvicorn/gunicorn from terminal.
```bash
python3 main.py
# (optional) you can also change the port by editing main.py
# port=8000
```
```bash
uvicorn app:app --host 0.0.0.0 --port 8000
```
Don't forget to open port if you're running this from a VPS.

## 