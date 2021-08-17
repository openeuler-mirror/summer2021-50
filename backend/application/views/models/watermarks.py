from application import db
from datetime import datetime


class Watermark(db.Model):
    __tablename__ = "watermark"
    nwid = db.Column(db.String(256), primary_key=True, comment="水印密码")
    hun = db.Column(db.String(64), db.ForeignKey("user.hun"), comment="用户名")
    watermark_created_time = db.Column(db.DateTime, default=datetime.now())

    watermark_name = db.Column(db.String(64), comment="水印图像名")
    watermark_type = db.Column(db.String(64), comment="水印图像类型")
    watermark_size = db.Column(db.Integer, comment="水印图像大小")
    watermark_m = db.Column(db.Integer, comment="水印图像长")
    watermark_n = db.Column(db.Integer, comment="水印图像宽")

    origin_name = db.Column(db.String(64), comment="原图像名")
    origin_type = db.Column(db.String(64), comment="原图像名")
    origin_size = db.Column(db.Integer, comment="原图像名")

    c_parameter_x = db.Column(db.String(256), comment="x")
    c_parameter_mu = db.Column(db.String(256), comment="mu")
    c_parameter_k = db.Column(db.String(256), comment="k")

    watermarked_image_path = db.Column(db.Text, comment="已加水印图像地址")

    def __repr__(self):
        return '<Watermarkid %r>' % self.nwid


if __name__ == "__main__":
    print(datetime.now())
