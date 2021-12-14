from django.db import models

# Create your models here.
class Result(models.Model):
    __tablename__ = "result"
    id = models.AutoField(primary_key=True)
    player_id = models.IntegerField(null=False)
    pazzletime = models.IntegerField(null=False)
    movecount = models.IntegerField(null=False)
    playdatetime = models.DateTimeField(auto_now_add=True)
    resigned = models.IntegerField(default=0)
 
class Player(models.Model):
    __tablename__ = "player"
    player_id = models.AutoField(primary_key=True)
    name = models.CharField(max_length=255,null=False)
    created_dt = models.DateTimeField(auto_now_add=True)
    current = models.IntegerField(default=0)

