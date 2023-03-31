const express = require('express');
const router = express.Router();
const path = require('path');

// Serve static files from the public directory
router.use(express.static(path.join(__dirname, '..', 'public')));

module.exports = router;