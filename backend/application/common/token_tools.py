from itsdangerous import TimedJSONWebSignatureSerializer as Serializer
from flask import current_app
import functools
from flask import request, jsonify


def login_required(view_func):
    @functools.wraps(view_func)
    def verify_token(*args, **kwargs):
        try:
            # 在请求头上拿到token
            token = request.headers["Authorization"]
        except Exception:
            # 没接收的到token,给前端抛出错误
            # 这里的code推荐写一个文件统一管理。这里为了看着直观就先写死了。
            return jsonify(code=4101, msg='缺少参数token')
        s = Serializer(current_app.config["SECRET_KEY"])
        if "Authorization" in request.headers:
            token = request.headers["Authorization"]
            try:
                s.loads(token)
            except Exception:
                return jsonify(code=4101, msg="登录已过期")
        return view_func(*args, **kwargs)
    return verify_token

def create_token(api_user):
    # 第一个参数是内部的私钥，这里写在共用的配置信息里了，如果只是测试可以写死
    # 第二个参数是有效期(秒)
    s = Serializer(current_app.config["SECRET_KEY"], expires_in=3600)
    # 接收用户id转换与编码
    token = s.dumps({"id": api_user}).decode("ascii")
    return token


def verify_token(token):
    # 参数为私有秘钥，跟上面方法的秘钥保持一致
    s = Serializer(current_app.config["SECRET_KEY"])
    try:
        # 转换为字典
        data = s.loads(token)
    except Exception:
        return None
    # 拿到转换后的数据，根据模型类去数据库查询用户信息
    return data