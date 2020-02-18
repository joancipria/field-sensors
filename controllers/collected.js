'use strict'

// Sensors Schema
const SensorData = require('../models/collected')

/*---------------
--- FUNCTIONS ---
----------------*/

// Get sensors by field
function getSensorData (req, res) {
  let sensorId = req.params.sensorId

  SensorData.find({'sensor': sensorId }, (err, data) => {
    if (err) return res.status(500).send({message: `Error al realizar la petición: ${err}`})
    if (!data) return res.status(404).send({message: `No hay sensores`})

    res.status(200).send({ data })
  })
}


module.exports = {
  getSensorData
}
