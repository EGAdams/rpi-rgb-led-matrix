Here is the pseudo code for the View Controllers involved in a project that I need debugged:

## 1. `HtmlChatButtonClickHandler` Class

### **Class Definition**
- **Implements**: 
  - `ConversationIdFetchDelegate`
  - `WKScriptMessageHandler`
  - `SignInViewControllerDelegate`
- **Properties**:
  - `viewController`: Reference to the main `ViewController`.
  - `navigationController`: Weak reference to `UINavigationController`.
  - `conversation_id_getter`: Object for retrieving conversation IDs.
  - `signInWindow`: Window used for presenting `SignInViewController`.
  - `messagesWindow`: Window for managing message views.

### **Methods**
#### **Initialization**
- Initializes with `ViewController`.
- Logs references to the `ViewController` and `NavigationController`.

#### **Handle Script Messages**
- **`userContentController(_:didReceive:)`**:
  - Logs the message.
  - Checks registration status via `McbaConfiguration`.
  - If not registered:
    - Presents `SignInViewController`.
  - If registered:
    - Checks user roles:
      - Admin: Loads `AdminViewController`.
      - Regular User: Loads `MessageViewController`.

#### **Present Sign-In View Controller**
- Creates a new `UIWindow` for modal presentation of `SignInViewController`.
- Configures routing and initializes UI.

#### **Show Message View Controller**
- Retrieves or generates a `conversation_id`.
- Instantiates and presents `MessageViewController`.
- Routes presentation based on availability of:
  - `navigationController`
  - `viewController`
  - Topmost view controller.

#### **Delegate Handling**
- **`didFinishSigningIn(signInViewController:)`**:
  - Dismisses `SignInViewController`.
  - Updates `conversation_id` and Firebase token.

---

## 2. `SignInViewController` Class

### **Class Definition**
- **Inherits**: `UIViewController`
- **Implements**:
  - `UITextFieldDelegate`
  - `ConversationIdFetchDelegate`
- **Properties**:
  - Handles for managing UI and routing.
  - Delegate references for `clickHandler`.

### **Methods**
#### **User Interaction**
- Handles sign-in and registration flows.

#### **Helpers**
- **`storeLogin`**: Saves user credentials to `UserDefaults`.
- **`finalizeLogin`**: Finalizes user authentication.

#### **Lifecycle**
- **`viewDidLoad`**: Initializes UI components.

---

## 3. `ViewController` Class

### **Class Definition**
- **Inherits**: `UIViewController`
- **Implements**:
  - `WKNavigationDelegate`
  - `WKUIDelegate`
  - `AdminViewControllerDelegate`
- **Properties**:
  - Web-related: `webView`, `progressBar`.
  - Handlers: `htmlClickHandler`, `updateManager`.

### **Methods**
#### **Refresh WebView**
- Configures the `WKWebView` and loads web content.

#### **Navigation Events**
- **`webView(didFinish:)`**: Logs loading completion.
- **`webView(shouldStartLoadWith:)`**: Intercepts and logs JS console messages.

#### **Lifecycle**
- **`viewDidLoad`**: Sets up UI and initializes handlers.

---

## **Interactions Between Classes**

1. **`HtmlChatButtonClickHandler` and `ViewController`**:
   - Handler requires `ViewController` for navigation and presentation.

2. **`HtmlChatButtonClickHandler` and `SignInViewController`**:
   - Presents the sign-in flow when the user is not registered.

3. **`HtmlChatButtonClickHandler` and `AdminViewController`**:
   - Pushes `AdminViewController` for admin users.

4. **`SignInViewController` and `UserDefaults`**:
   - Stores credentials and retrieves configuration settings.

5. **`ViewController` and Web Content**:
   - Configures the `WKWebView` with custom JavaScript handlers.

---

## **Debugging Tips**

1. **Ensure Navigation Controller Availability**:
   - Verify `navigationController` is non-nil before pushing controllers.

2. **Check UserDefaults Keys**:
   - Confirm required keys (`email`, `conversation_id`, `chat_admin_email`) are present.

3. **Storyboard Identifiers**:
   - Validate storyboard identifiers for all view controllers.

4. **JavaScript Bridge**:
   - Debug message handling in `userContentController(_:didReceive:)`.

5. **View Lifecycle**:
   - Add breakpoints or logging to trace `viewDidLoad` and modal presentations.

---

*Save this file as `Debugging_ViewControllers.md` for reference.*



Please rewrite the above document to include pseudo code from the following code.  If there are interactions between any of the objects, please note where and why they occur in the new markdown document.  I am trying to use this document to attempt to debug the interatctions between these View Controller Objects.

```swift
class HtmlRewardsClickHandler: NSObject, WKScriptMessageHandler, SignInViewControllerDelegate {
    weak var viewController: ViewController?
    weak var navigationController: UINavigationController?
    var rewards_view_controller_already_loaded = false
    
    init( viewController: ViewController ) {  self.viewController = viewController }
    
    func userContentController( _ userContentController: WKUserContentController, didReceive message: WKScriptMessage ) {
        print( "message.name: \(message.name)" )
        let mainInstance = McbaConfiguration.sharedInstance()                 // Reward button clicked!
        if ( !mainInstance.isRegistered()) {
            self.presentSignInViewController( completion: { print( "inside completion: of HtmlRewardsClickHandler::presentSignInViewController...  " )})
        } else {
            let defaults = UserDefaults.standard
            // DefaultsPrinter.printDefaults() // this is noisy and only prints keys.
            let device_email = defaults.string( forKey: "email"           )
            // let admin_email  = defaults.string(forKey: "chat_admin_email" )
            if ( device_email != nil ) {
                print ( "loading the RewardsViewController... " )
                let mLibraryBundle = Bundle( identifier: "awm.ios.mcba.mLibrary" )
                let storyboard = UIStoryboard(name: "mcbaStoryboard", bundle: mLibraryBundle)
                if let rewardsViewController = storyboard.instantiateViewController(withIdentifier: "RewardsViewController") as?
                    RewardsViewController {
                    if let navController = viewController?.navigationController {
                        navController.pushViewController( rewardsViewController, animated: true )
                    } else {
                        print( "Navigation controller is nil!" )
                    }
                }}}}
    
    func presentSignInViewController(completion: (() -> Void)? = nil) {
        let mLibraryBundle = Bundle( identifier: "awm.ios.mcba.mLibrary" )
        let storyboard = UIStoryboard( name: "mcbaStoryboard", bundle: mLibraryBundle )
        if let signInViewController = storyboard.instantiateViewController( withIdentifier: "SignInViewController" ) as? SignInViewController {
            signInViewController.modalPresentationStyle = .fullScreen
            signInViewController.delegate = self
            signInViewController.web_refresher = self.viewController
            self.viewController?.present( signInViewController, animated: true, completion: nil )}} // need a nav controller here.  no back button.
    
    func didFinishSigningIn( signInViewController: SignInViewController ) {
        /*
            There was a trick used in the html handler because the sign in window could not be presented for the life of me.
            I kept geting the window not in hier... message.  Somehow 4o made a window out of thin air.  If we ever have to
            do that again, use this code.
         */
        print( "hopefully we won't need to make a window out of thin air here..." )
    }
}
```

The Html Chat Button click handler goes to the sign in view controller if the user is not signed in.  After the sign in, the chat room is shown.  If the "Gift" buttong is clicked without the user being signed in, the Html Rewards Handler is run and send the user to the Sign In page.  After the user is signed in after clicking the "Gift" or rewards button (same button), I get the following error in the Xcode console:
```
Attempt to present <mLibrary.FadeAlertViewController: 0x105805db0> on <UINavigationController: 0x103056800> (from <mLibrary.ViewController: 0x103056e00>) whose view is not in the window hierarchy.
```

How is it that the Html click handler follows the correct path, but the Rewards or "Gift" click handler does not have the view in the hierarchy?
