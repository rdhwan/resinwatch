# class UID(BaseModel):
#     uid: str = Field(...)

#     class Config:
#         schema_extra = {"example": {"uid": "123456789"}}

#     @validator("uid")
#     def check_uid(cls, v):
#         if len(v) < 8:
#             raise ValueError("UID must be at least 8 digits long")
#         return v


def ResponseModel(data, message):
    return {"code": 200, "message": message, "data": data}


def ErrorResponseModel(code, message):
    return {"code": code, "message": message}
