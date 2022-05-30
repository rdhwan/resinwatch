import os
import secrets

from dotenv import load_dotenv
from fastapi import Depends, FastAPI, HTTPException, Request
from fastapi.security import HTTPBasic, HTTPBasicCredentials
from starlette.status import HTTP_401_UNAUTHORIZED

from models import UID, ResponseModel, ErrorResponseModel
from api import genshinapi

load_dotenv()
app = FastAPI(openapi_url=None, docs_url=None)

security = HTTPBasic()


def login(credentials: HTTPBasicCredentials = Depends(security)):
    correct_username = secrets.compare_digest(
        credentials.username, os.getenv("APP_USERNAME")
    )
    correct_password = secrets.compare_digest(
        credentials.password, os.getenv("APP_PASSWORD")
    )
    if not (correct_username and correct_password):
        raise HTTPException(
            status_code=HTTP_401_UNAUTHORIZED,
            detail="Incorrect username or password",
            headers={"WWW-Authenticate": "Basic"},
        )
    return True


@app.get("/api/{uid}")
async def get_data(uid: str, login: bool = Depends(login)):
    if secrets.compare_digest(uid, os.getenv("UID")):
        try:
            data = await genshinapi.get_data(
                {"ltuid": os.getenv("LTUID"), "ltoken": os.getenv("LTOKEN")}, uid
            )
            return ResponseModel(data=data, message="Success")
        except:
            return ErrorResponseModel(code=500, message="Internal Server Error")

    return ErrorResponseModel(code=404, message="Wrong UID")
