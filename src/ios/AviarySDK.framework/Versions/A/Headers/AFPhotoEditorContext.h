//
//  AFPhotoEditorContext.h
//  AviarySDK
//
//  Created by Cameron Spickert on 3/6/12.
//  Copyright (c) 2012 Aviary, Inc. All rights reserved.
//

@class AFPhotoEditorSession;

/**
 Photo Editor Contexts are generated by calling the createContextWithImage: or createContextWithImage:maxSize: methods of AFPhotoEditorSession. 
 A context encapsulates an input image and an output size. If not specified, the output size defaults to the image size. In the Basic SDK, the 
 output size is scaled down to a maximum of 3MP (if necessary, maintaining aspect ratio). There is no such limitation in the Premium SDK.

 Calling render: on a context causes all modifications tracked in the AFPhotoEditorSession (which tracks user generated photo-editing actions in 
 an AFPhotoEditorController) to be replayed on the context's input image asynchronously. When rendering is complete, the completion block passed 
 into render: will be called with the finalized image, whose size will match the context's size. If the session contains no tracked actions (i.e., the user made 
 no edits before pressing "Done" or pressed "Cancel"), the result image will be `nil`. Note: render: may only be called once per context.

 By default, contexts take advantage of GPU acceleration. If an input image is too large to be represented as an OpenGL texture, an attempt to render 
 an image will cause the context to silently fall back to CPU rendering. Maximum texture size is measured by the maximum dimension of an image, and 
 both the width and height of the input image must be less than this maximum dimension. iPhone 3GS, iPhone 4, and iPad 1 have a maximum texture 
 dimension of 2048 (4MP for a square image), while iPhone 5, iPhone 4S, iPad 2, and new iPads have a maximum texture dimension of 4096 (16MP for a square image).

 Behavior varies between GPU and CPU rendering when a context is created before a session is closed. If the context is able to use the GPU, it will 
 wait until the session is closed before beginning to render the output image. This is because GPU processing is very fast for most user action sets, and 
 waiting until the session is closed to begin rendering saves memory and processing power. If the context falls back to using the CPU, it will render 
 actions on the image in the background as the user applies them in the SDK (by pressing the "Apply" button).

 Some things to note about using contexts:
 
 - We recommend choosing a maximum output resolution that suits the purposes of your application to provide the best experience for your users.
 - Processing large images can consume a lot of memory and time. It may be necessary to free up resources and/or display an activity indicator
   while a context is executing.
 - To calculate the approximate maximum amount of memory a context could use, multiply the dimensions of the output size and multiply the result
   by 4. For example, a context with a maximum output size of 1500x1500 will consume approximately 9mb of memory for the processing duration.
 */
@interface AFPhotoEditorContext : NSObject

/// The session that the context was generated from.
@property (nonatomic, weak, readonly) AFPhotoEditorSession *session;

/// The size that the context's image will be output at.
@property (nonatomic, assign, readonly) CGSize size;

/// Specifies if the context's rendering has been cancelled.
@property (nonatomic, assign, readonly, getter=isCanceled) BOOL canceled;

/// Specifies if the context's session has been modified.
@property (nonatomic, assign, readonly, getter=isModified) BOOL modified;

/** 
 Specifies whether rendering has begun on the context.
 
 If YES, any render: call will have no effect.
*/
@property (nonatomic, assign, readonly) BOOL hasBegunRendering;

/**
 Replays all actions tracked by the generating AFPhotoEditorSession on the context's image. 
 
 The completion block will not be called until after the `session` is closed, i.e. until the user has closed the AFPhotoEditorController that generated the 
 `session`. If the user pressed "Cancel" or took no actions before pressing "Done", the `result` UImage in the completion block will be nil. Otherwise, it 
 will contain the rendered image. The context will try to render the image on the GPU, but it will fall back to the CPU if necessary. See the discussion in 
 AFPhotoEditorContext for more details.
 
 Note: This method may be called once per context, as it would not make sense to run the same actions on the same image twice. Check hasBegunRendering to 
 see if render has been called.
 
 @param completion The block to be called when the image's render is complete. 
 
 @warning Calling this method from any thread other in the main thread may result in undefined behavior.
 */
- (void)render:(void (^)(UIImage *result))completion;

/**
 Cancels the context's rendering.
 
 @warning Calling this method from any thread other in the main thread may result in undefined behavior.
*/
- (void)cancelRendering;

@end
