<?php

ini_set('session.cookie_lifetime', 3600);
session_start();

function displayPage($welcomeMessage = '', $showLogout = false) {
	?>
	<!DOCTYPE html>
	<html lang="en">
	<head>
		<meta charset="UTF-8">
		<meta name="viewport" content="width=device-width, initial-scale=1.0">
		<title>User Login Form</title>

	</head>
	<body>
		<div class="container">
			<h1>User Session Demo</h1>

			<?php if ($welcomeMessage): ?>
				<div class="welcome">
					<h2><?php echo htmlspecialchars($welcomeMessage); ?></h2>
					<p>Your session is active and stored via cookies.</p>
				</div>
			<?php endif; ?>

			<?php if ($showLogout): ?>
				<form action="login.php" method="POST">
					<input type="hidden" name="action" value="logout">
					<input type="submit" value="Logout">
				</form>
			<?php else: ?>
				<form action="login.php" method="POST">
					<div class="form-group">
						<label for="username">Username:</label>
						<input type="text" id="username" name="username" required>
					</div>
					<input type="submit" value="Login">
				</form>
			<?php endif; ?>
		</div>
	</body>
	</html>
	<?php
}

if ($_SERVER['REQUEST_METHOD'] == 'POST') {
	if (isset($_POST['action']) && $_POST['action'] == 'logout') {
		session_destroy();

		if (isset($_COOKIE[session_name()])) {
			setcookie(session_name(), '', time() - 3600, '/');
		}

		displayPage();
		exit();
	}

	if (isset($_POST['username']) && !empty(trim($_POST['username']))) {
		$username = trim($_POST['username']);

		$_SESSION['username'] = $username;
		$_SESSION['login_time'] = date('Y-m-d H:i:s');

		displayPage("Welcome back, " . $username . "!", true);
	} else {
		displayPage("Please enter a valid username.");
	}

} else {
	if (isset($_SESSION['username'])) {
		$username = $_SESSION['username'];
		$loginTime = isset($_SESSION['login_time']) ? $_SESSION['login_time'] : 'Unknown';
		displayPage("Welcome back, " . $username . "! (Logged in since: " . $loginTime . ")", true);
	} else {
		displayPage();
	}
}
?>
