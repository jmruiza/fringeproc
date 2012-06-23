# -*- coding: utf-8 -*-
"""
Main graphic user interface of fringeproc application.

Author: Julio C. Estrada
"""
from ui_mainwin import Ui_UnwrapGUI
from unwrapimage import UnwrapImage
from unwrappixmapitem import UnwrapPixmapItem
from dlgunwrap import DlgUnwrap
from PyQt4 import QtCore, QtGui
from PyQt4.QtCore import Qt, QObject, pyqtSlot, pyqtSignal
from readimage import Image
import fringeproc as fringes
import numpy as np
import cv2
import os

class Constrains(QObject):
  init_state = 0
  data_loaded = 1
  data_unloaded = 2
  data_saved = 3
  data_processing = 4
  data_processed = 5
  data_invalid = 6
  
  action_canceled = 7
  action_executed = 8
  action_accepted = 9
  
  file_open = 10
  file_closed = 11
  file_saved = 12
  
  user_interacting = 13

  busy_state = 14
  
  _constrains = set([init_state, data_loaded, data_saved, data_unloaded,
                     data_processing, data_processed, data_processed,
                     action_accepted, action_canceled, action_canceled,
                     file_closed, file_open, file_saved,
                     user_interacting, busy_state])
  _state = set([init_state])
  
  stateChanged = pyqtSignal(int)
    
  def __init__(self, parent=None):
    super(Constrains, self).__init__(parent)
    
  
  def setState(self, uistate):
    if isinstance(uistate, list):
      state = set(uistate)
    elif isinstance(uistate, int):
      state = set([uistate])
    else:
      raise TypeError, "Argument must be int or list"
    
    if len(state & self._constrains) > 0:
      self._state = state
    else:
      raise KeyError, "Attemptinng to ser an unknown state"
    
    self.stateChanged.emit(self._constrains)
        
  def getState(self):
    return self._state
  
  def All(self):
    return Constrains._constrains.copy()
    
class UnwrapGUI(QtGui.QMainWindow, Ui_UnwrapGUI):
  """ Graphic user interface for fringeproc.

  This user interface offers menus to interact with the user. 
  
  Properties:
   * _image: A reference to the PixmapItem that contains the image data
     being shown in the application scene.
     :type: UnwrapPixmapItem
   * _scene: The graphics scene that shows the graphic elements in the user
     interface.
     :type: QGraphicsScene
   * _system_dlg: Instance of the current system dialog.
     
  Author: Julio C. Estrada <julio@cio.mx>
  """
  # PixmapItem having the image data
  _image = object
  # The graphics scene used to show the image
  _scene = None
  # The current istance of the system's dialog
  _system_dlg = None
  # Mantains the interface state
  _actionState = Constrains()
  # A reference to the current process
  _process = None
  
  def __init__(self, parent=None):
    """ UnwrapGUI(parent=None)
    Constructs the graphic user interface.
    
    Initializes the scene and the graphics view.
    
    Parameters:
     * parent: A widge parent. If it is `None` it means that it is the
       principal widget.
       :type: QWidget
    
    Author: Julio C. Estrada <julio@cio.mx>
    """
    super(UnwrapGUI,self).__init__(parent)
    self.setupUi(self)
    self._connectActions()
    
    self._actionState.setState(Constrains.init_state)
    
    self._scene = QtGui.QGraphicsScene()
    self._graphicsView.setScene(self._scene)
    
  def _connectActions(self):
    """
    _connectActions()
    Connects the signals and slots of menu actions.
    
    Author: Julio C. Estrada <julio@cio.mx>
    """
    self._mnuFileClose.triggered.connect(self._onClose)
    self._mnuFileOpen.triggered.connect(self._onOpen)
    self._mnuFileOpenMask.triggered.connect(self._onOpenMask)
    self._mnuFileQuit.triggered.connect(self._onQuit)
    self._mnuFileSave.triggered.connect(self._onSave)
    self._mnuPhaseUnwrapping.triggered.connect(self._onPhaseUnwrapping)
    self._mnuPhaseDemodulation.triggered.connect(self._onPhaseDemodulation)
    
    self._mnuFileClose.constrainedTo = set([Constrains.data_loaded, 
                                           Constrains.file_open])
    self._mnuFileOpen.constrainedTo = self._actionState.All() - set([self._actionState.busy_state])
    self._mnuFileOpenMask.constrainedTo = set([Constrains.data_loaded,
                                              Constrains.file_open])
    self._mnuFileQuit.constrainedTo = self._actionState.All()
    self._mnuFileSave.constrainedTo = set([Constrains.data_loaded,
                                          Constrains.file_open])
    self._mnuPhaseUnwrapping.constrainedTo = set([Constrains.data_loaded])
    self._mnuPhaseDemodulation.constrainedTo = set([Constrains.data_loaded])


    self._actionState.stateChanged.connect(self._onUpdateInterface)
    
  def _onPhaseUnwrapping(self):
    """
    _onPhaseUnwrapping()
    Called when the user selects 'Phase unwrapping' from the system menu.
    
    Author: Julio C. Estrada <julio@cio.mx>
    """
    self._guiState.setState(Constrains.action_executed)

    if self._image is not None:
      self._guiState.setState(Constrains.action_accepted)
      self._guiState.setState(Constrains.data_processing)
      
      self._system_dlg = DlgUnwrap(self)
      dlg = self._system_dlg
      dlg.exec_()
      
      self._guiState.setState(Constrains.data_processed)
    
    self._guiState.setState(Constrains.action_canceled)
              
  def _onPhaseDemodulation(self):
    """
    _onPhaseDemodulation()
    Called when the user selects 'Phase demodulation' from the system menu.
    
    Author: Julio C. Estrada <julio@cio.mx>
    """
    self._actionState.setState(Constrains.data_processed)
    
  @pyqtSlot()
  def _onOpen(self):
    """
    _onOpen()
    Called when the user selects 'Open' from file menu.
    
    It launches a dialog for the user in order to select the file to open.
    When the file is opened it is created a QPixmapItem that holds the image
    data as a numpy array.
    
    Author: Julio C. Estrada <julio@cio.mx>
    """
    fileFilters = ["Image files (*.png *.jpg *.tif *.bmp)",
                   "Flt files (*.flt)"]
    fname = QtGui.QFileDialog.getOpenFileName(self, "Open image data", 
                                              QtCore.QDir.currentPath(),
                                              fileFilters[0]+';;'+fileFilters[1])
    self.setCursor(Qt.BusyCursor)
    self._process = Image()
    self._process.finished.connect(self._onProcessOpen)
    self._process.openFile(fname)
    self.statusBar().showMessage("Loading image file!!!")

    self._actionState.setState(Constrains.busy_state)
    
  @pyqtSlot(int)
  def _onProcessOpen(self, state):
    if self._process.getState() == self._process.image_loaded:
      for item in self._scene.items():
        self._scene.removeItem(item)
      self._image = UnwrapPixmapItem(self._process.getData())
      self._scene.addItem(self._image)
      self._image.setMoveEventHandler(self._onImageCursorOver)
      self._graphicsView.setSceneRect(self._scene.itemsBoundingRect())
        
      self._process.finished.disconnect(self._onProcessOpen)
      self.setCursor(Qt.ArrowCursor)
      self.statusBar().showMessage("")

      self._actionState.setState([Constrains.file_open, Constrains.data_loaded])
      
  @pyqtSlot()
  def _onOpenMask(self):
    """ _onOpenMask()
    Called when user selects 'Open mask' from file menu in order to apply this
    mask to an already loaded image data.
    
    Author: Julio C. Estrada <julio@cio.mx>
    """
    fileFilters = "Image files (*.png *.jpg *.tif *.bmp)"
    fname = QtGui.QFileDialog.getOpenFileName(self, "Open mask image", 
                                              QtCore.QDir.currentPath(),
                                              fileFilters)
    if(fname!='' and self._image!=None):
      image = self._image.getImage().getData('reference')
      self._openImage(fname, 'keep')
      if self._image != None:
        mask = self._image.getImage().getData('reference')/255
        data = image*mask
        self._image.setImage(data)
        self._actionState.setState(Constrains.data_loaded)

  def _onClose(self):
    self._actionState.setState([Constrains.file_closed, 
                                Constrains.data_unloaded])

  def _onSave(self):
    fileFilters = "Image files (*.png *.jpg *.tif)"
    fname = QtGui.QFileDialog.getSaveFileName(self, "Save data", 
                                              QtCore.QDir.currentPath(),
                                              fileFilters)
    if(fname[0]!=''):
      #self._image.save(fname[0])
      self._actionState.setState([Constrains.data_loaded, 
                                  Constrains.file_saved])
      
  
  def _onQuit(self):
    self.close()
    
  def _onImageCursorOver(self, pos):
    """_onImageCursosOver(pos)
    Called when the cursor pases over the image.

    Parameters:
     * pos is the (x,y) position
       :type: QPointF
    
    Notes:
     * This is not an slot, this is a callback function.
     
    Author: Julio C. Estrada
    """
    x = int(pos.x())
    y = int(pos.y())
    data = self._image.getImage().getData('reference')
    try:
      text = "(" + str(x) + ", " + str(y) + ") = " + \
        str(data[y,x])
    except IndexError:
      text = "Out of range!"
    stBar = self.statusBar()
    stBar.showMessage(text)
    
  def getScene(self):
    """ getScene()
    Returns the graphics scene of the user interface.
    
    The graphics scene is where al data to be processed is shown in a graphics
    view. Use this graphics scene to implement user interactions when needed.
    
    Return:
     * scene: The graphics scene
       :type: QGraphicsScene
    """
    return self._scene
  
  @pyqtSlot(int)
  def _onUpdateInterface(self, state):
    print "Update interface with state " + str(self._actionState.getState())
    
    for atribute in dir(self):
      if hasattr(getattr(self, atribute), 'constrainedTo'):
        self._constrainsOk(getattr(self, atribute))
    
  def _constrainsOk(self, obj):
    state = self._actionState.getState()
    if len(obj.constrainedTo & state)>0:
      obj.setEnabled(True)
      return
    obj.setEnabled(False)