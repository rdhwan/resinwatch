def ResponseModel(data, message):
    return {"code": 200, "message": message, "data": data}


def ErrorResponseModel(code, message):
    return {"code": code, "message": message}
