from pprint import pprint

from flask import (request, redirect, 
                url_for, render_template, 
                flash, session)
from flask_paginate import Pagination, get_page_parameter

from puzzle15 import app, db
from puzzle15.models import Player, Result

from sqlalchemy import text



page_dict = {"results": "Results", "players": "Players",
            "rankings": "Rankings", "statistics": "Statistics"}

context = {
    "page_dict":page_dict
    }


rank_type = ["Latest", "Moves", "Time"]

def get_played_time_str(t):
    pzt = int(t)
    hours = f'{(pzt//360000)}:' if (pzt//6000) > 60 else ""
    return f'{hours}{(pzt//6000)%60}:{(pzt//100)%60:02}.{pzt%60:02}'

def get_pzt_str(t):
    pzt = int(t)
    return f'{pzt//6000}:{(pzt//100)%60:02}.{pzt%60:02}'

@app.route('/')
def show_entries():
    entries = Player.query.order_by(Player.player_id.desc()).all()
    context_local = dict(
        entries=entries,
        **context
    )
    return render_template('show_entries.html', **context_local)

@app.route('/index2')
def index2():
    entries = Player.query.order_by(Player.player_id.asc()).all()
    context_local = dict(
        player=entries,
        **context
    )
    return render_template('index2.html', **context_local)

@app.route('/players', methods=['GET'])
def players():
    players = Player.query.all()

    context_local = dict(
        players=players,
        **context
    )
    return render_template('players.html', **context_local)
    
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
    result = db.session.execute(text("select" +
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
        ";"))
    # 各行を辞書に変換する関数
    def row_to_dict(row):
        # キーと値のペアを定義
        keys = ['player_id', 'name', 'play_count', 'played_time', 'best_time', 'worst_time', 'avg_time', 'best_movecount', 'worst_movecount', 'avg_movecount']
        
        # タプルの要素を辞書に変換
        return {keys[i]: value for i, value in enumerate(row)}

    # 結果セットから辞書に変換
    statistics = [row_to_dict(row) for row in result.fetchall()]
    for row_dict in statistics:
        row_dict['played_time'] = get_played_time_str(row_dict['played_time'])
        row_dict['best_time'] = get_pzt_str(row_dict['best_time'])
        row_dict['worst_time'] = get_pzt_str(row_dict['worst_time'])
        row_dict['avg_time'] = get_pzt_str(row_dict['avg_time'])
        row_dict['avg_movecount'] = '%.2f' % (row_dict['avg_movecount'],)

    context_local = dict(
        **context,
        statistics=statistics, col_dict=col_dict
    )
    return render_template('statistics.html', **context_local)

@app.route('/rankings')
def rankings():
    return redirect('/rankings/Latest')

@app.route('/rankings/<current_type>')
def rankings_typed(current_type):
    # current_type = "Latest"
    # s = "This is Rankings Page. Coming Soon..."
    s = ""
    num_title = "順位"
    if current_type == "Latest":
        num_title = "番号"
        result = Result\
            .query.order_by(Result.id.desc())\
            .limit(100)\
            .all()
    elif current_type == "Moves":
        result = Result\
            .query.order_by(Result.movecount.asc())\
            .limit(100)\
            .all()
    elif current_type == "Time":
        result = Result\
            .query.order_by(Result.pazzletime.asc())\
            .limit(100)\
            .all()
    else:
        s = "Unknown Ranking Type..."
        result = []
    context_local = dict(
        **context,
        rank_type=rank_type,
        num_title=num_title,
        result=result,
        current_type=current_type, s=s
    )
    return render_template('rankings.html', **context_local)


@app.route('/results', methods=['GET', 'POST'])
def results():
    if request.method == 'GET':
        per_page = 10
        result = Result.query.order_by(Result.id.asc()).all()
        page = request.args.get(get_page_parameter(), type=int, default=1)
        res = result[(page-1)*per_page: page*per_page]
        pagination = Pagination(page=page, total=len(result),  per_page=per_page, css_framework='bootstrap5')
    context_local = dict(
        **context,
        num_title="番号",
        pagination=pagination, paged_results=res, page=page
    )
    return render_template('results.html', **context_local)




@app.route('/add', methods=['POST'])
def add_entry():
    entry = Player(
            name=request.form['name']
            )
    db.session.add(entry)
    db.session.commit()
    flash('New entry was successfully posted')
    return redirect(url_for('show_entries'))



