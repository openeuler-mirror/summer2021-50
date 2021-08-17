import base64
import logging
from application.common.token_tools import create_token, login_required, verify_token
import json
from application.common.secgear import passwordEncryption, embedding
from flask import jsonify
import time
from application.views.models.users import User
from application.views.models.watermarks import Watermark
from sqlalchemy import and_, or_
from application import db
from application.common.utils import base64_encode, init_paramters
from datetime import datetime
import os

PARAMETERS = init_paramters()
IMAGE_PATH = "application/storage/images/"

if not os.path.exists(IMAGE_PATH):
    os.mkdir(IMAGE_PATH)


def timestamp_verify(request):
    try:
        ts = request["ts"]
        print(time.time() - float(ts))
        return True
    except Exception as err:
        logging.log(level=0, msg=str(err))
        return jsonify(code=3000, msg="后台错误: Timestamp Check Error {}".format(str(err)))


def phone_verfiy(phone, code):
    try:
        return True
    except Exception as err:
        logging.log(level=0, msg=str(err))
        return jsonify(code=3000, msg="后台错误: Phone Check Error {}".format(str(err)))


def check_password(request, hun=None):
    request = json.loads(request.get_data())
    if not timestamp_verify(request):
        jsonify(code=4101, msg="请求超时")
    try:
        if not hun:
            hun = request["hun"]
        cpw = request["cpw"]
        user = User.query.get(hun)
        if user:
            npw = passwordEncryption(cpw, PARAMETERS["CD"], PARAMETERS["N"], PARAMETERS["CKU"])
            if user.npw == npw:
                token = create_token(hun)
                return jsonify(code=0, msg="成功", data={"ts": time.time()}, token=token)
            else:
                return jsonify(code=4103, msg="密码输入错误")
        else:
            return jsonify(code=4103, msg="用户名错误")
    except Exception as err:
        logging.log(level=0, msg=str(err))
        return jsonify(code=3000, msg="后台错误: Password Validation Error {}".format(str(err)))


def add_user(request):
    request = json.loads(request.get_data())
    if not timestamp_verify(request):
        jsonify(code=4101, msg="请求超时")
    try:
        hun = request["hun"]
        cpw = request["cpw"]
        phone = request["phone"]
        code = request["code"]
        if User.query.get(hun):
            return jsonify(code=4105, msg="用户名已存在")
        if User.query.filter(User.phone == base64_encode(phone)).first():
            return jsonify(code=4105, msg="手机号已存在")
        if not phone_verfiy(phone, code):
            jsonify(code=4103, msg="验证码错误")
        npw = passwordEncryption(cpw, PARAMETERS["CD"], PARAMETERS["N"], PARAMETERS["CKU"])
        user = User(
            hun=hun,
            npw=npw,
            user_created_time=datetime.now(),
            phone=base64_encode(phone))
        db.session.add(user)
        db.session.commit()
        token = create_token(hun)
        return jsonify(code=0, msg="成功", data={"ts": time.time()}, token=token)
    except Exception as err:
        logging.log(level=0, msg=str(err))
        return jsonify(code=3000, msg="后台错误: Add User Error {}".format(str(err)))


def get_embed_parameters(request, hun):
    request = json.loads(request.get_data())
    if not timestamp_verify(request):
        jsonify(code=4101, msg="请求超时")
    try:
        cwid = request["cwid"]
        npw = User.query.get(hun).npw
        x, mu, k, nwid = embedding(cwid, npw, PARAMETERS["CD"], PARAMETERS["N"], PARAMETERS["CKU"], PARAMETERS["CKW"])
        watermark = Watermark(
            watermark_created_time=datetime.now(),
            nwid=nwid,
            hun=hun,
            watermark_name=request.get("watermark_name", ""),
            watermark_type=request.get("watermark_type", ""),
            watermark_size=request.get("watermark_size", ""),
            watermark_m=request.get("watermark_m", ""),
            watermark_n=request.get("watermark_n", ""),
            origin_name=request.get("origin_name", ""),
            origin_type=request.get("origin_type", ""),
            origin_size=request.get("origin_size", ""),
            c_parameter_x=x,
            c_parameter_mu=mu,
            c_parameter_k=k,
        )
        db.session.add(watermark)
        db.session.commit()
        return jsonify(code=0, msg="成功", data={"x": x, "mu": mu, "k": k, "ts": time.time()})
    except Exception as err:
        logging.log(level=0, msg=str(err))
        return jsonify(code=3000, msg="后台错误: Get Parameters Error {}".format(str(err)))


def add_watermarked_image(request, hun):
    try:
        user_path = os.path.join(IMAGE_PATH, hun)
        if not os.path.exists(user_path):
            os.mkdir(user_path)
        cwid = request.form['cwid']
        npw = User.query.get(hun).npw
        _, _, _, nwid = embedding(cwid, npw, PARAMETERS["CD"], PARAMETERS["N"], PARAMETERS["CKU"], PARAMETERS["CKW"])
        watermark = Watermark.query.get(nwid)
        image_path = os.path.join(user_path, nwid)
        with open(image_path, "wb") as f:
            f.write(request.files['result'].read())
        watermark.watermarked_image_path = image_path
        db.session.add(watermark)
        db.session.commit()
        return jsonify(code=0, msg="成功", data={"ts": time.time()})
    except Exception as err:
        print(str(err))
        return jsonify(code=3000, msg="后台错误: Add Watermarked Image Error {}".format(str(err)))


def get_extract_parameters(request, hun):
    request = json.loads(request.get_data())
    if not timestamp_verify(request):
        jsonify(code=4101, msg="请求超时")
    try:
        cwid = request["cwid"]
        npw = User.query.get(hun).npw
        _, _, _, nwid = embedding(cwid, npw, PARAMETERS["CD"], PARAMETERS["N"], PARAMETERS["CKU"], PARAMETERS["CKW"])
        watermark = Watermark.query.get(nwid)
        return jsonify(code=0, msg="成功", data={"x": watermark.c_parameter_x,
                                               "mu": watermark.c_parameter_mu,
                                               "k": watermark.c_parameter_k,
                                               "name": watermark.watermark_name,
                                               "type": watermark.watermark_type,
                                               "m": watermark.watermark_m,
                                               "n": watermark.watermark_n,
                                               "ts": time.time()})
    except Exception as err:
        print(str(err))
        return jsonify(code=3000, msg="后台错误: Get Watermarked Image Error {}".format(str(err)))
