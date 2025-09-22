import subprocess
import threading
import time
import os
from flask import Flask, jsonify, request
from flask_cors import CORS

app = Flask(__name__)
CORS(app)
from flask import send_from_directory

@app.route("/")
def root():
    return send_from_directory(os.path.join(os.path.dirname(__file__), "static"), "index.html")

@app.route("/<path:p>")
def static_files(p):
    return send_from_directory(os.path.join(os.path.dirname(__file__), "static"), p)

EXEC_PATH = os.path.abspath(os.path.join(os.path.dirname(__file__), '..', 'build', 'Release', 'ConcurrencySimulator.exe'))
# for Linux: EXEC_PATH = "/path/to/build/ConcurrencySimulator"

jobs = {}
jobs_lock = threading.Lock()
job_id_counter = 0

def run_process(job_id, mode):
    logdir = os.path.join(os.path.dirname(__file__), "logs")
    os.makedirs(logdir, exist_ok=True)
    logfile = os.path.join(logdir, f"job_{job_id}.log")
    with open(logfile, "wb") as f:
        proc = subprocess.Popen([EXEC_PATH, "--mode", str(mode)], stdout=f, stderr=subprocess.STDOUT)
        with jobs_lock:
            jobs[job_id]["pid"] = proc.pid
            jobs[job_id]["status"] = "running"
        proc.wait()
        with jobs_lock:
            jobs[job_id]["status"] = "finished"
            jobs[job_id]["returncode"] = proc.returncode

@app.route("/start", methods=["POST"])
def start_simulation():
    global job_id_counter
    data = request.get_json() or {}
    mode = int(data.get("mode", 0))
    if mode not in (1,2,3,4):
        return jsonify({"error": "mode must be 1..4"}), 400

    with jobs_lock:
        job_id_counter += 1
        jid = job_id_counter
        jobs[jid] = {"mode": mode, "status": "starting", "pid": None, "returncode": None, "start_time": time.time()}
    t = threading.Thread(target=run_process, args=(jid, mode), daemon=True)
    t.start()
    return jsonify({"job_id": jid})

@app.route("/jobs", methods=["GET"])
def list_jobs():
    with jobs_lock:
        return jsonify({jid: dict(info) for jid, info in jobs.items()})

@app.route("/metrics", methods=["GET"])
def get_metrics():
    metrics_file = os.path.join(os.path.dirname(__file__), "metrics.json")
    if os.path.exists(metrics_file):
        import json
        with open(metrics_file, "r") as f:
            try:
                metrics = json.load(f)
            except Exception:
                return jsonify({"error": "metrics file is not valid JSON"}), 500
        return jsonify(metrics)
    else:
        return jsonify({"note": "metrics not available - add C++ code to write web/metrics.json periodically"}), 404

@app.route("/logs/<int:job_id>", methods=["GET"])
def get_log(job_id):
    logfile = os.path.join(os.path.dirname(__file__), "logs", f"job_{job_id}.log")
    if not os.path.exists(logfile):
        return jsonify({"error": "no log"}), 404
    with open(logfile, "r", errors="ignore") as f:
        content = f.read()
    return jsonify({"log": content})

@app.route("/stop/<int:job_id>", methods=["POST"])
def stop_job(job_id):
    return jsonify({"error": "stop not implemented; implement cancellation in C++ and call kill PID"}), 501

if __name__ == "__main__":
    app.run(host="0.0.0.0", port=5000, debug=True)
