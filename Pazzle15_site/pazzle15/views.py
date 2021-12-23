from pprint import pprint

from flask import (request, redirect, 
                url_for, render_template, 
                flash, session)
from flask_paginate import Pagination, get_page_parameter

from pazzle15 import app, db
from pazzle15.models import Player, Result


page_dict = {"players":"Players", "results":"Results", "rankings":"Rankings", "statistics":"Statistics"}

def get_played_time_str(t):
    pzt = int(t)
    return f'{pzt//6000}:{(pzt//100)%60:02}.{pzt%60:02}'
def get_pzt_str(t):
    pzt = int(t)
    return f'{pzt//6000}:{(pzt//100)%60:02}.{pzt%60:02}'

@app.route('/')
def show_entries():
    entries = Player.query.order_by(Player.player_id.desc()).all()
    return render_template('show_entries.html', page_dict=page_dict, entries=entries)

@app.route('/index2')
def index2():
    entries = Player.query.order_by(Player.player_id.asc()).all()
    return render_template('index2.html', page_dict=page_dict, player=entries)

@app.route('/players', methods=['GET'])
def players():
    players = Player.query.all()
    return render_template('players.html', page_dict=page_dict, players=players)
    
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
    return redirect(url_for('players', page_dict=page_dict))

@app.route('/statistics')
def statistics():
    col_dict = {
        'player_id':'番号',
        'name':'名前',
        'play_count':'プレイ回数',
        'played_time':'総プレイ時間',
        'best_time':'ベストタイム',
        'worst_time':'ワーストタイム',
        'avg_time':'平均タイム',
        'best_movecount':'最小手数',
        'worst_movecount':'最多手数',
        'avg_movecount':'平均手数' 
    }
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
    statistics = [dict(row) for row in result]
    for row_dict in statistics:
        row_dict['best_time'] = get_pzt_str(row_dict['best_time'])
        row_dict['worst_time'] = get_pzt_str(row_dict['worst_time'])
        row_dict['avg_time'] = get_pzt_str(row_dict['avg_time'])
    return render_template('statistics.html',statistics=statistics, page_dict=page_dict, col_dict=col_dict)

@app.route('/rankings')
def rankings():
    s = "This is Rankings Page. Coming Soon..."
    return render_template('rankings.html', page_dict=page_dict, s=s)

@app.route('/results', methods=['GET', 'POST'])
def results():
    if request.method == 'GET':
        per_page = 10
        result = Result.query.order_by(Result.id.asc()).all()
        page = request.args.get(get_page_parameter(), type=int, default=1)
        res = result[(page-1)*per_page: page*per_page]
        pagination = Pagination(page=page, total=len(result),  per_page=per_page, css_framework='bootstrap5')
    return render_template('results.html',pagination = pagination, page_dict=page_dict, paged_results=res, page=page)

@app.route('/add', methods=['POST'])
def add_entry():
    entry = Player(
            name=request.form['name']
            )
    db.session.add(entry)
    db.session.commit()
    flash('New entry was successfully posted')
    return redirect(url_for('show_entries'))



