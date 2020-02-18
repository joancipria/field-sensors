'use strict'

const express = require('express')

// Controllers
const fieldCtrl = require('../controllers/field')
const sensorCtrl = require('../controllers/sensor')
const collectedCtrl = require('../controllers/collected')
const userCtrl = require('../controllers/user')

// Auth
const auth = require('../middlewares/auth')

// Api
const api = express.Router()

api.get('/product', auth, fieldCtrl.getProducts)

/*------------------- 
--- FIELDS ROUTES ---
--------------------*/

// Get field by Id
api.get('/field/:fieldId', fieldCtrl.getField)

// Get fields by owner
api.get('/fields/:ownerEmail', auth, fieldCtrl.getFieldsByOwner)



/*------------------- 
--- SENSOR ROUTES ---
--------------------*/

// Get sensor by Id
//api.get('/sensor/:sensorId', sensorCtrl.getField)

// Get sensors by field
api.get('/field/:fieldId/sensors',auth, sensorCtrl.getSensorByField)



/*------------------------ 
--- SENSOR DATA ROUTES ---
-------------------------*/
api.get('/field/sensor/:sensorId', auth, collectedCtrl.getSensorData)



api.post('/product', auth, fieldCtrl.saveProduct)
api.put('/product/:fieldId', auth, fieldCtrl.updateProduct)
api.delete('/product/:fieldId', auth, fieldCtrl.deleteProduct)
api.post('/signup', userCtrl.signUp)
api.post('/signin', userCtrl.signIn)
api.get('/private', auth, (req, res) => {
  res.status(200).send({ message: 'Tienes acceso' })
})

module.exports = api
