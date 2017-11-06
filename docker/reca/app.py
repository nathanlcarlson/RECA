from flask import Flask, render_template, flash, request
from wtforms import Form, TextField, TextAreaField, validators, StringField, SubmitField
import os
import socket
import subprocess

app = Flask(__name__)
app.config.from_object(__name__)
app.config['SECRET_KEY'] = '7d441f27d441f27567d441f2b6176a'
dbname = "db"
class ReusableForm(Form):
    name = TextField('Name:', validators=[validators.required()])

@app.route("/", methods=['GET', 'POST'])
def hello():
    form = ReusableForm(request.form)

    print form.errors
    if request.method == 'POST':
        beta = request.form['beta']
        L =request.form['width']
        freq = request.form['freq']
        steps = request.form['steps']
        subprocess.Popen(["./reca",
                          dbname,
                          L,
                          beta,
                          freq,
                          steps
                         ],
                          stdout=subprocess.PIPE,
                          stderr=subprocess.STDOUT)


    return render_template('hello.html', form=form)

if __name__ == "__main__":
    app.run(host='0.0.0.0', port=80)

