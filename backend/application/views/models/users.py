from application import db
from datetime import datetime


class User(db.Model):
    __tablename__ = "user"
    hun = db.Column(db.String(64), primary_key=True, comment="用户名")
    npw = db.Column(db.String(256), comment="密码")
    phone = db.Column(db.String(16), unique=True, comment="手机")
    user_created_time = db.Column(db.DateTime, default=datetime.now())
    watermarks = db.relationship("Watermark", backref="user", lazy="dynamic")
