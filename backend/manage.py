from application import init_app, db
# from flask_script import Manager
# from flask_migrate import Migrate, MigrateCommand

app = init_app("dev")


# 使用终端脚本工具启动和管理fla
# sk
# manager = Manager(app)
#
# # 启用数据迁移工具
# Migrate(app, db)
# # 添加数据迁移的命令到终端脚本工具中
# manager.add_command('db', MigrateCommand)

# 导入模型[为了进行数据迁移]
from application.views.models import users
from application.views.models import watermarks




if __name__ == '__main__':
    app.run()
