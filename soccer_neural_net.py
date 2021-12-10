#predict the results of soccer games based on player data. 

import pickle
import numpy as np
import plotly.graph_objects as go
import keras
import math

from keras.models import Sequential
from keras.layers import Dense

with open('soccer_data.pickle', 'rb') as f:
    (x_train, y_train), (x_test, y_test) = pickle.load(f)

unscaled_y_test = y_test.copy()
unscaled_x_test = x_test.copy()
# print(y_test[0])
y_mean = y_train.mean(axis=0)
y_std = y_train.std(axis=0)
y_train = (y_train - y_mean) / y_std
y_test = (y_test - y_mean) / y_std

# print(x_test)
# print(y_test)

# for i in range(0, len(x_train)):
#     if x_train[i][5]:

# Press the green button in the gutter to run the script.

import plotly.io as pio
pio.renderers.default = "browser"

if __name__ == '__main__':
    model = Sequential()
    model.add(Dense(128, input_shape=(22,), activation='relu', name='dense_1'))
    model.add(Dense(4, activation='linear', name='dense_output'))
    model.compile(optimizer='adam', loss='mse', metrics=['mae'])
    #model.summary()
    history = model.fit(x_train, y_train,
              batch_size=128,
              epochs=140,
              verbose=False,
              validation_data=(x_test, y_test))
    score = model.evaluate(x_test, y_test, verbose=0)
    fig = go.Figure()
    fig.add_trace(go.Scattergl(y=history.history['loss'],
                               name='Train'))
    fig.add_trace(go.Scattergl(y=history.history['val_loss'],
                               name='Valid'))
    fig.update_layout(height=500, width=700,
                      xaxis_title='Epoch',
                      yaxis_title='Loss')
    fig.show()

    #print(history.history['mae'])
    fig = go.Figure()

    fig.add_trace(go.Scattergl(y=history.history['mae'],
                               name='Train'))
    fig.add_trace(go.Scattergl(y=history.history['val_mae'],
                               name='Valid'))
    fig.update_layout(height=500, width=700,
                      xaxis_title='Epoch',
                      yaxis_title='Mean Absolute Error')
    fig.show()
    group_results = model.predict(x_test)
    unscaled_group_results = group_results * y_std + y_mean

    prediction_mses = []
    for i in range(0, 4):
        prediction_mses.append(keras.backend.get_value(keras.losses.mean_squared_error([x[i] for x in y_test], [x[i] for x in group_results])))

    #print(prediction_mses)
    rounded_mses = []
    for i in range(0, 4):
        rounded_mses.append(keras.backend.get_value(keras.losses.mean_squared_error([x[i] for x in unscaled_y_test], [round(x[i]) for x in unscaled_group_results])))

    for i in range(0, 4):
        pred_standard_dev = math.sqrt(rounded_mses[i])
        #print(pred_standard_dev)


    # print(group_results)
    result = model.predict(x_train[0:1])
    # print(y_train[0] * y_std + y_mean)
    result = result * y_std + y_mean
    # print(result)
    print('Test MSE:', score[0])
    print('Test MAE:', score[1])

    train_score = model.evaluate(x_train, y_train, verbose=0)
    print('Train MSE:', train_score[0])
    print('Train MAE:', train_score[1])

    import shap

    explainer = shap.DeepExplainer(model, x_train)
    shap_values = explainer.shap_values(x_test)
    #shap.summary_plot(shap_values, x_test, plot_type='bar', class_names=["Goals Scored", "Goals Conceded", "Drinks Consumed", "Money Earned/Lost"])
    shap.summary_plot(shap_values[0], x_test, plot_type="dot", class_names=["Goals Scored"])
    #shap.dependence_plot(0, shap_values[1], x_test)
