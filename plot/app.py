# -*- coding: utf-8 -*-
import dash
import sys
import dash_core_components as dcc
import dash_html_components as html
import numpy as np
import plotly.graph_objs as go
import logging
import os
import pandas as pd
from file_loader import load

logger = logging.getLogger()
handler = logging.StreamHandler(sys.stdout)
formatter = logging.Formatter(
    '%(asctime)s %(levelname)-8s %(message)s')
handler.setFormatter(formatter)
logger.addHandler(handler)
logger.setLevel(logging.DEBUG)

metric_names = ['min', 'max', 'max_ind', 'average', 'median', 'variance', 'variance_bounded', 'left_bound', 'right_bound']

#df = load(os.path.expanduser('~/prao-data/output'))
#df.to_csv('azaza.csv')

df = pd.read_csv('azaza.csv')  # TODO: remove
df = df[df['band_num'] == 5]

external_stylesheets = ['https://codepen.io/chriddyp/pen/bWLwgP.css']

app = dash.Dash(__name__, external_stylesheets=external_stylesheets)

app.layout = html.Div([
    dcc.Graph(id='graph-with-slider'),
    dcc.Slider(
        id='ray-slider',
        min=df['ray_num'].min(),
        max=df['ray_num'].max(),
        value=df['ray_num'].min(),
        marks={str(ray_num): str(ray_num) for ray_num in df['ray_num'].unique()}
    )
])


@app.callback(
    dash.dependencies.Output('graph-with-slider', 'figure'),
    [dash.dependencies.Input('ray-slider', 'value')])
def update_figure(selected_ray):
    logging.debug('Selected ray_num = %d', selected_ray)
    filtered_df = df[df['ray_num'] == selected_ray]
    traces = []
    for i in filtered_df.metric_num.unique():
        df_by_metric = filtered_df[filtered_df['metric_num'] == i]
        traces.append(go.Scatter(
            x=df_by_metric['ts'],
            y=df_by_metric['value'],

            #text='Metric ' + metric_names[i],
            mode='lines',
            opacity=0.7,
            marker={
                'size': 15,
                'line': {'width': 0.5, 'color': 'white'}
            },
            name=metric_names[i],
        ))

    return {
        'data': traces,
        'layout': go.Layout(
            xaxis={'title': 'Metrics values for band_num = 5 by ray'},
            yaxis={'title': 'Metrics', 'range': [20, 90]},
            margin={'l': 40, 'b': 40, 't': 10, 'r': 10},
            legend={'x': 0, 'y': 1},
            hovermode='closest'
        )
    }


if __name__ == '__main__':
    app.run_server(debug=True)
