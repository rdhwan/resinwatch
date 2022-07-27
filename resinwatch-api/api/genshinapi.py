import genshin


async def get_data(token: dict, uid: str) -> dict:
    gclient = genshin.Client(cookies=token, uid=uid)
    account = await gclient.get_game_accounts()
    notes = await gclient.get_notes(uid)

    data = {
        "username_lvl": account[0].nickname + " AR" + str(account[0].level),
        "uid": uid,
        "resin": f"{notes.current_resin}/{notes.max_resin}",
        "commission": f"{notes.completed_commissions}/{notes.max_commissions}",
    }

    return data
