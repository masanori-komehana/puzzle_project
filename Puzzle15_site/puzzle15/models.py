import datetime
from puzzle15 import db

class Result(db.Model):
    __tablename__ = "result"
    id = db.Column(db.Integer, primary_key=True, autoincrement=True)
    player_id = db.Column(db.Integer, 
            db.ForeignKey('player.player_id'), nullable=False)
    pazzletime = db.Column(db.Integer, nullable=False)
    movecount = db.Column(db.Integer, nullable=False)
    playdatetime = db.Column(db.DateTime(timezone=True), default=db.func.now())
    resigned = db.Column(db.Integer, server_default="0")
    player = db.relationship('Player')
    def __repr__(self):
        return f'<Result id={self.id} time={self.get_pzt_str()} moves={self.movecount}>'
    def get_pzt_str(self):
        pzt = int(self.pazzletime)
        return f'{pzt//6000}:{(pzt//100)%60:02}.{pzt%60:02}'
    def get_playdatetime_jst(self):
        datetime_utc = datetime.datetime.strptime(str(self.playdatetime) + "+0000", "%Y-%m-%d %H:%M:%S%z")
        datetime_jst = datetime_utc.astimezone(datetime.timezone(datetime.timedelta(hours=+9)))
        timestamp_jst = datetime.datetime.strftime(datetime_jst, '%Y-%m-%d %H:%M:%S')
        return timestamp_jst
    def get_keys(self):
        self.keys = ["名前", "手数", "タイム"]
        self.dict = {"名前": self.player.name, "手数":self.movecount, "タイム":self.get_pzt_str()}
        for k in self.keys:
            yield k


class Player(db.Model):
    __tablename__ = "player"
    player_id = db.Column(db.Integer, primary_key=True, autoincrement=True)
    name = db.Column(db.String, nullable=False)
    created_dt = db.Column(db.DateTime(timezone=True), default=db.func.now())
    current = db.Column(db.Integer, server_default="0")
    def __repr__(self):
        return '<Player id={id} name={name!r}>'.format(
                id=self.player_id, name=self.name)
    def get_created_dt_jst(self):
        datetime_utc = datetime.datetime.strptime(str(self.created_dt) + "+0000", "%Y-%m-%d %H:%M:%S%z")
        datetime_jst = datetime_utc.astimezone(datetime.timezone(datetime.timedelta(hours=+9)))
        timestamp_jst = datetime.datetime.strftime(datetime_jst, '%Y-%m-%d %H:%M:%S')
        return timestamp_jst
    def is_current(self):
        return self.current==1

def init():
    db.create_all()


if __name__ == '__main__':
    results = Result.query.all()
    for result in results:
        print(result)
        print(result.player)
