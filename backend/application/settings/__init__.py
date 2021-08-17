from redis import StrictRedis
import random
import base64
import os


class Config(object):
    """项目配置核心类"""
    # 调试模式
    DEBUG = True

    # todo 配置日志
    pass

    # mysql数据库的配置信息
    SQLALCHEMY_DATABASE_URI = 'mysql://secgear:secgear2021@www.fishworld.site:3306/secgear_user?charset=utf8'  # 动态追踪修改设置，如未设置只会提示警告
    SQLALCHEMY_TRACK_MODIFICATIONS = False
    # 查询时会显示原始SQL语句
    SQLALCHEMY_ECHO = False

    # 配置redis
    REDIS_HOST = '127.0.0.1'
    REDIS_PORT = 5000

    # 设置密钥，可以通过 base64.b64encode(os.urandom(48)) 来生成一个指定长度的随机字符串
    SECRET_KEY = base64.b64encode(os.urandom(48))

    # flask_session的配置信息
    SESSION_TYPE = "redis"  # 指定 session 保存到 redis 中
    SESSION_USE_SIGNER = True  # 让 cookie 中的 session_id 被加密签名处理
    SESSION_REDIS = StrictRedis(host=REDIS_HOST, port=REDIS_PORT, db=1)  # 使用 redis 的实例
    PERMANENT_SESSION_LIFETIME = 24 * 60 * 60  # session 的有效期，单位是秒
