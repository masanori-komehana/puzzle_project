# Generated by Django 3.2.10 on 2021-12-14 01:57

from django.db import migrations, models


class Migration(migrations.Migration):

    initial = True

    dependencies = [
    ]

    operations = [
        migrations.CreateModel(
            name='Player',
            fields=[
                ('player_id', models.IntegerField(primary_key=True, serialize=False)),
                ('name', models.CharField(max_length=255)),
                ('created_dt', models.DateTimeField(auto_now_add=True)),
                ('current', models.IntegerField(default=0)),
            ],
        ),
        migrations.CreateModel(
            name='Result',
            fields=[
                ('id', models.IntegerField(primary_key=True, serialize=False)),
                ('player_id', models.IntegerField()),
                ('pazzletime', models.IntegerField()),
                ('movecount', models.IntegerField()),
                ('playdatetime', models.DateTimeField(auto_now_add=True)),
                ('resigned', models.IntegerField(default=0)),
            ],
        ),
    ]