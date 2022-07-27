import os
import secrets

from api import genshinapi
from dotenv import load_dotenv
from fastapi import Depends, FastAPI, HTTPException
from fastapi.security import OAuth2PasswordBearer
from models import ResponseModel

load_dotenv()
app = FastAPI(openapi_url=None, docs_url=None)
oauth2_scheme = OAuth2PasswordBearer(tokenUrl=os.getenv("BEARER_TOKEN"))

USER_UID = os.getenv("UID")


@app.get("/api/{uid}")
async def get_data(uid: str, token: str = Depends(oauth2_scheme)):
    if not secrets.compare_digest(uid, USER_UID):
        return HTTPException(status_code=404, detail="Wrong UID")

    try:
        data = await genshinapi.get_data(
            {"ltuid": os.getenv("LTUID"), "ltoken": os.getenv("LTOKEN")}, uid
        )
        return ResponseModel(data=data, message="Success")

    except BaseException:
        return HTTPException(status_code=500, detail="Internal Server Error")
