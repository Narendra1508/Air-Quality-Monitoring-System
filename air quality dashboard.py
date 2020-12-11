# import libraries
import datetime
import re
from datetime import datetime as dt

import dash
import dash_core_components as dcc
import dash_html_components as html
import pandas as pd
import mysql.connector
import plotly.express as px
import plotly.graph_objects as go
import plotly.graph_objs as go
from dash.dependencies import Input, Output

# Connect to the MySQL database
db = mysql.connector.connect(host = 'localhost', user = 'root', passwd = '', db = 'air_quality')

# Check if connection was successful
if(db):
    print("Connection successful")
    cursor = db.cursor()
    cursor.execute("SELECT * FROM dht11")
    for x in cursor:
        print(x)
    cursor.close()
    db.close()
else:
    print("Connection unsuccessful")


fig = go.Figure()

X_TIME = df['TIME']
Y_EC = df['EC']

# Add traces
fig.add_trace(go.Scatter(x=X_TIME, y=Y_EC,
                    mode='lines+markers',
                    name='lines+markers'))

fig.update_layout(
    title="EC sensor data",
    xaxis_title="Timestamps",
    yaxis_title="EC (uS)"
)


fig2 = go.Figure()

X_TIME = df['TIME']
Y_RTD = df['RTD']

# Add traces
fig2.add_trace(go.Scatter(x=X_TIME, y=Y_RTD,
                    mode='lines+markers',
                    name='lines+markers'))

fig2.update_layout(
    title="Temperature sensor data",
    xaxis_title="Timestamps",
    yaxis_title="Temperature (°C)"
)

fig3 = go.Figure()

X_TIME = df['TIME']
Y_PH = df['PH']

# Add traces
fig3.add_trace(go.Scatter(x=X_TIME, y=Y_PH,
                    mode='lines+markers',
                    name='lines+markers'))

fig3.update_layout(
    title="pH sensor data",
    xaxis_title="Timestamps",
    yaxis_title="pH values"
)


app = dash.Dash()
app.layout = html.Div([
    html.Div(html.Label('Air quality Monitoring System'), style={'textAlign': 'center', 'font-size':'160%'}),
    html.Div([
        html.Div([
            html.Label('Date', style={'padding': 10}),
            # dcc.DatePickerSingle(
            #     id='my-date'),
            dcc.Input(
                id="input_date",
                placeholder="Date", style={'padding': 10}),
            # html.P([html.Br()]),

            html.Label('Location', style={'padding': 10}),
            dcc.Input(
                id="input_location",
                placeholder="Location", style={'padding': 10})
        ], style={'padding': 10}),
        html.Div([
            html.Div(
                dcc.Graph(id='c1',
                    figure=fig),
                    className="six columns",
                    style={"width":500, "margin": 25, 'display': 'inline-block'}
            ),
            html.Div(
                dcc.Graph(id='g2', 
                    figure=fig3), 
                    className="six columns",
                    style={"width":500, "margin": 25, 'display': 'inline-block'}
            )
        ])
    ], className="row"),
    html.Div([
        html.Div(
          dcc.Graph(id='g1', 
                    figure=fig2), 
                    className="six columns",
                    style={"width":500, "margin": 25, 'display': 'inline-block'}
        )
    ], className="row")
]
# , style={'backgroundColor':'aqua'}
)



if __name__ == '__main__':
    app.run_server(debug=True)
