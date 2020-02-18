'use strict'

const mongoose = require('mongoose')
const Schema = mongoose.Schema

const CollectedSchema = Schema({
  sensor: {
    type: mongoose.Schema.Types.ObjectId,
    ref: 'Sensor'
  },
  temperature: Number,
  salinity: Number,
  humidity: Number,
  lighting: Number,
  pressure: Number,
  date: Number
})

module.exports = mongoose.model('Collected', CollectedSchema)
