from flask import Flask, request, jsonify, render_template

app = Flask(__name__)

# Variables para almacenar los datos recibidos
current_temperature = None
current_humidity = None

@app.route('/')
def dashboard():
    # Ruta principal que servirá el dashboard
    return render_template('dashboard.html')

@app.route('/data', methods=['POST'])
def receive_data():
    global current_temperature, current_humidity
    data = request.get_json()
    
    # Guardar los valores de temperatura y humedad recibidos
    current_temperature = data['temperatura']
    current_humidity = data['humedad']
    
    print(f"Datos recibidos: Temperatura: {data['temperatura']}, Humedad: {data['humedad']}")
    return jsonify({"status": "Datos recibidos"}), 200

@app.route('/latest-data', methods=['GET'])
def latest_data():
    # Enviar los datos más recientes al frontend
    return jsonify({
        "temperatura": current_temperature,
        "humedad": current_humidity
    }), 200

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)
