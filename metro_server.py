import subprocess
from flask import Flask, request, jsonify
import os
import csv
from flask_cors import CORS

app = Flask(__name__)
CORS(app)

GRAPH_EXE = os.path.join(os.getcwd(), 'graph.exe')
CSV_FILE = os.path.join(os.getcwd(), 'processed_metro_edges_with_colors2.csv')

def run_graph_app(inputs):
    try:
        proc = subprocess.Popen([GRAPH_EXE], stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
        out, err = proc.communicate(inputs, timeout=10)
        return out
    except Exception as e:
        return str(e)

@app.route('/stations', methods=['GET'])
def get_stations():
    # Read unique stations from the CSV file
    stations = set()
    with open(CSV_FILE, newline='', encoding='utf-8') as csvfile:
        reader = csv.DictReader(csvfile)
        for row in reader:
            stations.add(row['Source'])
            stations.add(row['Destination'])
    stations = sorted(stations)
    return jsonify({'stations': stations})

@app.route('/distance', methods=['POST'])
def get_distance():
    data = request.get_json() or {}
    src = data.get('src', '')
    dest = data.get('dest', '')
    inputs = f'3\n{src}\n{dest}\n4\n'
    output = run_graph_app(inputs)
    return jsonify({'output': output})

@app.route('/path', methods=['POST'])
def get_path():
    data = request.get_json() or {}
    src = data.get('src', '')
    dest = data.get('dest', '')
    inputs = f'3\n{src}\n{dest}\n4\n'
    output = run_graph_app(inputs)
    return jsonify({'output': output})

@app.route('/map', methods=['GET'])
def get_map():
    output = run_graph_app('2\n4\n')
    return jsonify({'output': output})

if __name__ == '__main__':
    app.run(debug=True) 