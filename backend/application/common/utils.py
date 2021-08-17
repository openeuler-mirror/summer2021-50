import os
import json
import hashlib
import base64
import rsa
from application.common.secgear import initServer

CaptchaAppId = "2024376293"


def sha256(x):
    return hashlib.sha256(x.encode('utf-8')).hexdigest()


def base64_encode(x):
    return base64.b64encode(x.encode("utf-8")).decode()


def init_paramters(size = 1024):
    if os.path.exists("pubkey"):
        with open("pubkey", "r") as f:
            parameter = json.load(f)
    else:
        (public_key, private_key) = rsa.newkeys(size, exponent=3)
        public_key = hex(private_key.n) + "&" + hex(private_key.e)
        public_key = base64.b64encode(public_key.encode("utf-8"))
        public_key = public_key.decode("utf-8")
        CD, CKU, CKW = initServer(hex(private_key.d)[2:],hex(private_key.n)[2:])
        parameter = {
            "CD": CD,
            "CKU": CKU,
            "CKW": CKW,
            "N": hex(private_key.n)[2:],
            "public_key": public_key,
        }
        with open("pubkey", "w") as f:
            json.dump(parameter, f)
        del private_key
    return parameter


if __name__ == "__main__":
    print(init_paramters())
