from pprint import pprint

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

@app.route('/statistics')
def statistics():
    result = db.engine.execute("select" +
        "    player.player_id as 'player_id'," +
        "    player.name as 'name'," +
        "    count(*) as 'play_count'," +
        "    (sum(result.pazzletime)) as 'played_time'," +
        "    (min(result.pazzletime)) as 'best_time'," +
        "    (max(result.pazzletime)) as 'worst_time'," +
        "    (avg(result.pazzletime)) as 'avg_time'," +
        "    min(result.movecount) as 'best_movecount'," +
        "    max(result.movecount) as 'worst_movecount'," +
        "    avg(result.movecount) as 'avg_movecount' " +
        "from" +
        "    result " +
        "inner join " +
        "    player " +
        "on" +
        "    result.player_id=player.player_id " +
        "group by" +
        "    player.name " +
        "order by " +
        " player.player_id asc " +
        ";")
    pprint([dict(row) for row in result])
    return "This is Statistics Page. Coming Soon..."

@app.route('/paged_results')
def paged_results_0():
    return redirect(url_for('paged_results', page=1))

@app.route('/paged_results/<int:page>', methods=['GET'])
def paged_results(page=1):
    per_page = 10
    results = Result.query.order_by(Result.id.asc()).paginate(
        page, per_page, error_out=False)
    page_max = (results.total // per_page)
    return render_template('paged_results.html', page_max, per_page=per_page, paged_results=results, page=page)

@app.route('/add', methods=['POST'])
def add_entry():
    entry = Player(
            name=request.form['name']
            )
    db.session.add(entry)
    db.session.commit()
    flash('New entry was successfully posted')
    return redirect(url_for('show_entries'))



