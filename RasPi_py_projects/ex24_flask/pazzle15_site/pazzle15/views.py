from flask import request, redirect, url_for, render_template, flash
from pazzle15 import app, db
from pazzle15.models import Player

@app.route('/')
def show_entries():
    entries = Player.query.order_by(Player.player_id.desc()).all()
    return render_template('show_entries.html', entries=entries)

@app.route('/add', methods=['POST'])
def add_entry():
    entry = Player(
            name=request.form['name']
            )
    db.session.add(entry)
    db.session.commit()
    flash('New entry was successfully posted')
    return redirect(url_for('show_entries'))



