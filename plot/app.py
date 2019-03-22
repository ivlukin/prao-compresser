# -*- coding: utf-8 -*-
import dash
import sys
import dash_core_components as dcc
import dash_html_components as html
import plotly.graph_objs as go
import logging
import os
import click
import glob
from file_loader import load

logger = logging.getLogger()

metric_names = ['min', 'max', 'max_ind', 'average', 'median', 'variance', 'variance_bounded', 'left_bound', 'right_bound']

runtime_config = {}


def make_slider(param_name, slider_id):
    idx = '{}_num'.format(param_name)

    if param_name == 'metric':
        marks = {str(x): metric_names[x] for x in df[idx].unique()}
    else:
        marks = {str(x): str(x) for x in df[idx].unique()}

    return dcc.Slider(
        id='{}-slider-{}'.format(param_name, slider_id),
        min=df[idx].min(),
        max=df[idx].max(),
        value=df[idx].min(),
        marks=marks
    )


def make_centered_p(text):
    return html.P(text, style={'text-align': 'center', 'font-style': 'bold'})


logger = logging.getLogger()
handler = logging.StreamHandler(sys.stdout)
formatter = logging.Formatter(
    '%(asctime)s %(levelname)-8s %(message)s')
handler.setFormatter(formatter)
logger.addHandler(handler)
logger.setLevel(logging.DEBUG)


df = load(os.path.expanduser('~/prao-data/output'))
#df = load(os.path.expanduser('~/prao-data/011014_02_N1_00.pnt.processed'))
#df.to_csv('azaza_new.csv')
#df.to_csv('azaza.csv')

#df = pd.read_csv('azaza.csv')  # TODO: remove

external_stylesheets = ['https://codepen.io/chriddyp/pen/bWLwgP.css']

app = dash.Dash(__name__, external_stylesheets=external_stylesheets)

app.layout = html.Div([
    html.H1('PRAO Data Viewer'),
    html.I('That’s one small step for a man, but one giant leap for mankind. (Neil Armstrong)'),

    html.H2('Rays by Band & Metric'),
    dcc.Graph(id='graph-band-metric'),
    make_slider('band', 3),
    html.Br(),
    make_centered_p('band'),
    make_slider('metric', 3),
    html.Br(),
    make_centered_p('metric'),


    html.H2('Bands by Ray & Metric'),
    dcc.Graph(id='graph-ray-metric'),
    make_slider('ray', 2),
    html.Br(),
    make_centered_p('ray'),
    make_slider('metric', 2),
    html.Br(),
    make_centered_p('metric'),

    html.H2('Metrics by Band & Ray'),
    dcc.Graph(id='graph-band-ray'),
    make_slider('band', 1),
    html.Br(),
    make_centered_p('band'),
    make_slider('ray', 1),
    html.Br(),
    make_centered_p('ray'),

], style={'margin-left': '50px', 'margin-right': '50px'})


@app.callback(
    dash.dependencies.Output('graph-band-ray', 'figure'),
    [dash.dependencies.Input('ray-slider-1', 'value'), dash.dependencies.Input('band-slider-1', 'value')])
def update_figure_1(selected_ray, selected_band):
    filtered_df = df[(df['ray_num'] == selected_ray) & (df['band_num'] == selected_band)]
    traces = []
    for i in filtered_df.metric_num.unique():
        df_by_metric = filtered_df[filtered_df['metric_num'] == i]
        traces.append(go.Scatter(
            x=df_by_metric['ts'],
            y=df_by_metric['value'],
            x0=0,
            y0=0,
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
            xaxis={'range': [0, 360]},
            yaxis={'range': [0, 1000]},
            margin={'l': 40, 'b': 40, 't': 10, 'r': 10},
            legend={'x': 0, 'y': 1},
            hovermode='closest'
        )
    }


@app.callback(
    dash.dependencies.Output('graph-ray-metric', 'figure'),
    [dash.dependencies.Input('ray-slider-2', 'value'), dash.dependencies.Input('metric-slider-2', 'value')])
def update_figure_2(selected_ray, selected_metric):
    filtered_df = df[(df['ray_num'] == selected_ray) & (df['metric_num'] == selected_metric)]
    traces = []
    for i in filtered_df.band_num.unique():
        df_by_metric = filtered_df[filtered_df['band_num'] == i]
        traces.append(go.Scatter(
            x=df_by_metric['ts'],
            y=df_by_metric['value'],
            x0=0,
            y0=0,
            mode='lines',
            opacity=0.7,
            marker={
                'size': 15,
                'line': {'width': 0.5, 'color': 'white'}
            },
            name=str(i)))

    return {
        'data': traces,
        'layout': go.Layout(
            xaxis={'range': [0, 360]},  # TODO: 360 считать по df
            yaxis={'range': [0, 1000]},
            margin={'l': 40, 'b': 40, 't': 10, 'r': 10},
            legend={'x': 0, 'y': 1},
            hovermode='closest'
        )
    }


@app.callback(
    dash.dependencies.Output('graph-band-metric', 'figure'),
    [dash.dependencies.Input('band-slider-3', 'value'), dash.dependencies.Input('metric-slider-3', 'value')])
def update_figure_3(selected_band, selected_metric):
    filtered_df = df[(df['band_num'] == selected_band) & (df['metric_num'] == selected_metric)]
    traces = []
    for i in filtered_df.ray_num.unique():
        df_by_metric = filtered_df[filtered_df['ray_num'] == i]
        traces.append(go.Scatter(
            x=df_by_metric['ts'],
            y=df_by_metric['value'],
            x0=0,
            y0=0,
            mode='lines',
            opacity=0.7,
            marker={
                'size': 15,
                'line': {'width': 0.5, 'color': 'white'}
            },
            name=str(i),
        ))

    return {
        'data': traces,
        'layout': go.Layout(
            xaxis={'range': [0, 360]},
            yaxis={'range': [0, 1000]},
            margin={'l': 40, 'b': 40, 't': 10, 'r': 10},
            legend={'x': 0, 'y': 1},
            hovermode='closest'
        )
    }


@click.command('run')
@click.argument('dir_path', type=str)
def run(dir_path):
    runtime_config['dir_path'] = os.path.expanduser(dir_path)

    if not os.path.exists(runtime_config['dir_path']):
        logger.error('Directory %s does not exist, exiting', runtime_config['dir_path'])
        return

    logging.debug(glob.glob(os.path.expanduser(runtime_config['dir_path'] + "/*.processed")))
    runtime_config['files'] = map(os.path.basename, glob.glob(os.path.expanduser(runtime_config['dir_path'] + "/*.processed")))
    logging.debug(runtime_config['files'])

    app.run_server(debug=True)


if __name__ == '__main__':
    run()
