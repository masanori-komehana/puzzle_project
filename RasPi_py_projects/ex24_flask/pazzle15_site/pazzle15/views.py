from flask import request, redirect, url_for, render_template, flash
from pazzle15 import app, db
from pazzle15.models import Player, Result

@app.route('/')
def show_entries():
    entries = Player.query.order_by(Player.player_id.desc()).all()
    return render_template('show_entries.html', entries=entries)

@app.route('/index2')
def index2():
    entries = Player.query.order_by(Player.player_id.asc()).all()
    return render_template('index2.html', player=entries)

@app.route('/results')
def results():
    s = ""
    results = Result.query.all()
    return render_template('index3.html', results=results)

@app.route('/results2')
def results2():
    s = ""
    results = Result.query.all()
    return render_template('results.html', results=results)

@app.route('/players', methods=['GET'])
def players():
    players = Player.query.all()
    return render_template('players.html', players=players)
@app.route('/players', methods=['POST'])
def select_player():
    select_id = int(request.form['select_id'])
    player1 = Player.query.filter(Player.current==1).first()
    player1.current = 0
    current_id = player1.player_id
    player2 = Player.query.filter(Player.player_id == select_id).first()
    player2.current = 1
    db.session.add(player1)
    db.session.add(player2)
    db.session.commit()
    print(f"player change: {current_id} -> {select_id}")
    flash(f'Player Selected: {player2.name}')
    return redirect(url_for('players'))

@app.route('/add', methods=['POST'])
def add_entry():
    entry = Player(
            name=request.form['name']
            )
    db.session.add(entry)
    db.session.commit()
    flash('New entry was successfully posted')
    return redirect(url_for('show_entries'))



