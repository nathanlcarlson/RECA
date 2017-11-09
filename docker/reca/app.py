from flask import Flask, render_template, flash, request, make_response
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
        L = request.form['width']
        freq = request.form['freq']
        steps = request.form['steps']
        if request.form['submit'] == "Graph":
            
        if request.form['submit'] == "Run":
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

@app.route("/simple.png")
def simple():
    import datetime
    import StringIO
    import random

    from matplotlib.backends.backend_agg import FigureCanvasAgg as FigureCanvas
    from matplotlib.figure import Figure
    from matplotlib.dates import DateFormatter

    fig=Figure()
    ax=fig.add_subplot(111)
    x=[]
    y=[]
    now=datetime.datetime.now()
    delta=datetime.timedelta(days=1)
    for i in range(10):
        x.append(now)
        now+=delta
        y.append(random.randint(0, 1000))
    ax.plot_date(x, y, '-')
    ax.xaxis.set_major_formatter(DateFormatter('%Y-%m-%d'))
    fig.autofmt_xdate()
    canvas=FigureCanvas(fig)
    png_output = StringIO.StringIO()
    canvas.print_png(png_output)
    response=make_response(png_output.getvalue())
    response.headers['Content-Type'] = 'image/png'

    return response
if __name__ == "__main__":
    app.run(host='0.0.0.0', port=80)

