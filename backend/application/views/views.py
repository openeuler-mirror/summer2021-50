from application.common.token_tools import create_token, login_required, verify_token
from application.common.utils import CaptchaAppId, init_paramters
from application.views.views_utils import get_extract_parameters, add_watermarked_image, check_password, add_user, \
    get_embed_parameters
from flask import jsonify, request
import time
from application.views.models.watermarks import Watermark

from . import index_blu

public_key = init_paramters()["public_key"]


@index_blu.route("/tools/init", methods=["GET"])
def init():
    return jsonify(code=0, msg="成功", data={"PubKey": public_key, "AppId": CaptchaAppId})


@index_blu.route('/login', methods=["POST"])
def login():
    return check_password(request)


@index_blu.route('/register', methods=["POST"])
def register():
    return add_user(request)


@index_blu.route('/validate', methods=["POST"])
@login_required
def validate():
    hun = verify_token(request.headers["Authorization"])["id"]
    return check_password(request, hun)


@index_blu.route('/embed', methods=["POST"])
@login_required
def embed():
    hun = verify_token(request.headers["Authorization"])["id"]
    return get_embed_parameters(request, hun)


@index_blu.route('/add', methods=["POST"])
@login_required
def add():
    hun = verify_token(request.headers["Authorization"])["id"]
    return add_watermarked_image(request, hun)


@index_blu.route('/extract', methods=["POST"])
@login_required
def extract():
    hun = verify_token(request.headers["Authorization"])["id"]
    return get_extract_parameters(request, hun)


@index_blu.route('/personInfo', methods=["GET"])
@login_required
def getInfo():
    hun = verify_token(request.headers["Authorization"])["id"]
    datas = [{"time": i.watermark_created_time,
              "nwid": i.nwid,
              "watermark_name": i.watermark_name,
              "watermark_type": i.watermark_type,
              "watermark_size": i.watermark_size,
              "origin_name": i.origin_name,
              "origin_type": i.origin_type,
              "origin_size": i.origin_size,
              "images": i.watermarked_image_path,
              } for i in Watermark.query.filter(Watermark.hun == hun)]
    return jsonify(code=0, msg="成功", data={"datas": datas, "ts": time.time()})


@index_blu.route('/', methods=["GET"])
@login_required
def get_username():
    hun = verify_token(request.headers["Authorization"])
    return jsonify(code=0, msg="成功", data={"hun": hun, "ts": time.time()})
