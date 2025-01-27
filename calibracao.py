import numpy as np
import pandas as pd
from scipy.optimize import curve_fit
from statsmodels.tsa.arima.model import ARIMA

def calculate_pt_and_n(distances, rssi_values):
    """
    Calcula P_t e n usando ajuste por mínimos quadrados.

    Parâmetros:
    distances (list ou np.array): Lista de distâncias (em metros).
    rssi_values (list ou np.array): Lista de valores de RSSI (em dBm).

    Retorna:
    tuple: (P_t, n)
    """
    # Modelo de perda de caminho: RSSI = P_t - 10 * n * log10(d)
    def path_loss_model(d, Pt, n):
        return Pt - 10 * n * np.log10(d)

    # Ajustar o modelo aos dados
    popt, _ = curve_fit(path_loss_model, distances, rssi_values, bounds=([-np.inf, 0], [np.inf, np.inf]))

    # popt contém os valores ajustados de P_t e n
    Pt, n = popt
    return Pt, n

def optimize_alpha(rssi_values):
    """
    Otimiza o parâmetro alpha do filtro exponencial.

    Parâmetros:
    rssi_values (list ou np.array): Lista de valores de RSSI (em dBm).

    Retorna:
    float: Alpha otimizado.
    """
    def exponential_filter_error(alpha, rssi_values):
        filtered = rssi_values[0]  # Inicializa o filtro com o primeiro valor
        error = 0
        for i in range(1, len(rssi_values)):
            filtered = alpha * rssi_values[i] + (1 - alpha) * filtered
            error += (filtered - rssi_values[i]) ** 2
        return error

    # Otimizar alpha no intervalo [0, 1]
    from scipy.optimize import minimize_scalar
    result = minimize_scalar(exponential_filter_error, bounds=(0, 1), args=(rssi_values,), method='bounded')
    return result.x

def fit_arma_model(rssi_values, ar_order=3, ma_order=3):
    """
    Ajusta os coeficientes AR e MA usando o modelo ARMA.

    Parâmetros:
    rssi_values (list ou np.array): Lista de valores de RSSI (em dBm).
    ar_order (int): Ordem autoregressiva do modelo AR.
    ma_order (int): Ordem média móvel do modelo MA.

    Retorna:
    tuple: (ar_coeffs, ma_coeffs)
    """
    # Ajustar o modelo ARMA
    model = ARIMA(rssi_values, order=(ar_order, 0, ma_order))
    result = model.fit()

    # Retornar os coeficientes AR e MA
    return result.arparams, result.maparams

# Exemplo de uso: Ajustar com 5 arquivos CSV
file_paths = [
    'calibracao\data1.csv',  # Arquivo para 1 metro
    'calibracao\data3.csv',  # Arquivo para 3 metros
    'calibracao\data5.csv',  # Arquivo para 5 metros
    'calibracao\data10.csv',  # Arquivo para 10 metros
    'calibracao\data05.csv'   # Arquivo para 20 metros
]

distances = [0.5, 1, 3, 5, 10]
rssi_values = []

# Carregar RSSI de cada arquivo
for file_path in file_paths:
    data = pd.read_csv(file_path)
    rssi_values.append(data['RSSI'].dropna().astype(float).mean())

# Calcular P_t e n
Pt, n = calculate_pt_and_n(distances, np.array(rssi_values))

# Otimizar alpha
alpha = optimize_alpha(np.array(rssi_values))

# Ajustar ARMA
ar_coeffs, ma_coeffs = fit_arma_model(np.array(rssi_values))

# Resultados
print(f"P_t (RSSI a 1 metro): {Pt:.2f} dBm")
print(f"n (expoente de perda de caminho): {n:.2f}")
print(f"Alpha (filtro exponencial): {alpha:.2f}")
print(f"Coeficientes AR: {ar_coeffs}")
print(f"Coeficientes MA: {ma_coeffs}")
