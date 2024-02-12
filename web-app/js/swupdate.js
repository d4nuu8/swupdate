/*!
 * Copyright (C) 2017-2018 Weidmüller Interface GmbH & Co. KG
 * Stefan Herbrechtsmeier <stefan.herbrechtsmeier@weidmueller.com>
 *
 * SPDX-License-Identifier: MIT
 */

/* global $, Dropzone, WebSocket */

const StatusEnum = {
  IDLE: 'IDLE',
  START: 'START',
  RUN: 'RUN',
  SUCCESS: 'SUCCESS',
  FAILURE: 'FAILURE',
  DONE: 'DONE'
};

function isStatusInEnum(status){
  return (status in StatusEnum)
}

function restart () {
  $.post('restart', {}, function (data) {
    showRestart()
  })
}

function showRestart () {
  $('#swu-restart-modal').modal({ backdrop: 'static', keyboard: false })
  window.setTimeout(tryReload, 3000)
}

function tryReload () {
  $.ajax({
    cache: false,
    timeout: 1000,
    success: function (response) {
      window.location.reload(true)
    },
    error: function (xhr, textStatus, errorThrown) {
      if (textStatus === 'timeout') { tryReload() } else { window.setTimeout(tryReload, 1000) }
    }
  })
}

function updateStatus (status) {
  if(!isStatusInEnum(status)) return;
  $('#swu-idle').hide()
  $('#swu-success').hide()
  $('#swu-failure').hide()
  $('#swu-done').hide()
  $('#swu-run').hide()

  switch (status) {
    case StatusEnum.IDLE:
      $('#swu-idle').show()
      break
    case StatusEnum.START:
    case StatusEnum.RUN:
      $('#swu-run').show()
      break
    case StatusEnum.SUCCESS:
      $('#swu-success').show()
      break
    case StatusEnum.FAILURE:
      $('#swu-failure').show()
      break
    case StatusEnum.DONE:
      $('#swu-done').show()
      break
    default:
      break
  }
}

var updateProgressBarStatus = (function (status) {
  var s = ''

  return function (status) {
    if(!isStatusInEnum(status)) return;
    $('#swu-progress-bar')
      .removeClass('bg-danger bg-success progress-bar-animated')
    $('#swu-progress-spinner')
      .removeClass('fa-spinner fa-spin')
    $('#swu-progress-run').hide()

    switch (status) {
      case StatusEnum.START:
        updateProgressBar(0, '', '')
        break
      case StatusEnum.RUN:
        $('#swu-progress-bar').addClass('progress-bar-animated')
        $('#swu-progress-spinner')
          .addClass('fa-spinner fa-spin')
        $('#swu-progress-run').show()
        break
      case StatusEnum.SUCCESS:
        $('#swu-progress-bar')
          .addClass('bg-success')
        break
      case StatusEnum.FAILURE:
        if (s !== 'START' || s !== 'RUN') { updateProgressBar(0, '', '') }
        $('#swu-progress-bar')
          .addClass('bg-danger')
        break
      default:
        break
    }
    s = status
  }
})()

function updateProgressBar (percent, name, value) {
  $('#swu-progress-value').text(value)
  $('#swu-progress-name').text(name)
  $('#swu-progress-bar')
    .css('width', percent + '%')
    .attr('aria-valuenow', percent)
}

Dropzone.options.dropzone = {
  timeout: 0,
  clickable: true,
  acceptedFiles: '.swu',
  maxFilesize: 0
}

window.onload = function () {
  var protocol

  $('#swu-restart').click(restart)

  if (window.location.protocol === 'https:') { protocol = 'wss:' } else { protocol = 'ws:' }

  var ws = new WebSocket(protocol + '//' + window.location.host + window.location.pathname.replace(/\/[^/]*$/, '') + '/ws')

  ws.onopen = function (event) {
    updateStatus(StatusEnum.IDLE)
  }

  ws.onclose = function (event) {
    showRestart()
  }

  ws.onmessage = function (event) {
    var msg = JSON.parse(event.data)

    switch (msg.type) {
      case 'message':
        var p = $('<p></p>')
        p.text(msg.text)
        p.addClass('mb-1')
        if (msg.level <= 3) { p.addClass('text-danger') }
        $('#messages').append(p)
        break
      case 'status':
        updateStatus(msg.status)
        updateProgressBarStatus(msg.status)
        break
      case 'source':
        break
      case 'step':
        var percent = Math.round((100 * (Number(msg.step) - 1) + Number(msg.percent)) / Number(msg.number))
        var value = percent + '%' + ' (' + msg.step + ' of ' + msg.number + ')'

        updateProgressBar(percent, msg.name, value)
        break
    }
  }
}
